#include "sim808_mock.h"
#include <string.h>

/* Simple ring buffer for response lines */
#define MOCK_MAX_LINE_LEN   64
#define MOCK_MAX_LINES      8

static char  mock_lines[MOCK_MAX_LINES][MOCK_MAX_LINE_LEN];
static int   mock_head = 0;
static int   mock_tail = 0;
static int   initialized = 0;

static void mock_push_line(const char *s)
{
    if (!s) return;
    strncpy(mock_lines[mock_head], s, MOCK_MAX_LINE_LEN - 1);
    mock_lines[mock_head][MOCK_MAX_LINE_LEN - 1] = '\0';
    mock_head = (mock_head + 1) % MOCK_MAX_LINES;
    if (mock_head == mock_tail) {
        mock_tail = (mock_tail + 1) % MOCK_MAX_LINES; /* overwrite oldest */
    }
}

void sim808_mock_init(void)
{
    mock_head = 0;
    mock_tail = 0;
    initialized = 1;
    /* On power-up, SIM808 typically responds "OK" to basic AT */
}

void sim808_mock_send_command(const char *cmd)
{
    if (!initialized || !cmd) return;

    if (strcmp(cmd, "AT") == 0) {
        mock_push_line("OK");
    } else if (strcmp(cmd, "AT+CGATT?") == 0) {
        mock_push_line("+CGATT: 1");
        mock_push_line("OK");
    } else if (strcmp(cmd, "AT+CGNSPWR=1") == 0) {
        mock_push_line("OK");
    } else if (strcmp(cmd, "AT+CGNSINF") == 0) {
        /* Fake GPS info: 1,1,20250101...,lat,lon,... */
        mock_push_line("+CGNSINF: 1,1,20250101,4807.038,N,01131.000,E,0.0");
        mock_push_line("OK");
    } else {
        mock_push_line("ERROR");
    }
}

void sim808_mock_task(void)
{
    /* For now, nothing time-based; could simulate delays later */
}

int sim808_mock_get_line(char *out, int max_len)
{
    if (!initialized || !out || max_len <= 0) return 0;
    if (mock_head == mock_tail) return 0; /* no data */

    strncpy(out, mock_lines[mock_tail], max_len - 1);
    out[max_len - 1] = '\0';
    mock_tail = (mock_tail + 1) % MOCK_MAX_LINES;
    return 1;
}
