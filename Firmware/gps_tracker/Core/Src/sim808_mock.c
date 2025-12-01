#include "sim808_mock.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ===== MOCK RESPONSE BUFFER ===== */

static char g_mock_responses[512];
static uint16_t g_resp_idx = 0;

/* ===== INITIALIZATION ===== */

void sim808_mock_init(void)
{
    memset(g_mock_responses, 0, sizeof(g_mock_responses));
    g_resp_idx = 0;
}

/* ===== COMMAND PROCESSING ===== */

void sim808_mock_send_command(const char *cmd)
{
    if (!cmd) return;

    // Generate mock responses based on command
    if (strcmp(cmd, "AT") == 0) {
        sim808_mock_queue_response("OK");
    } else if (strcmp(cmd, "AT+CGATT?") == 0) {
        sim808_mock_queue_response("+CGATT: 1");
        sim808_mock_queue_response("OK");
    } else if (strcmp(cmd, "AT+CGNSPWR=1") == 0) {
        sim808_mock_queue_response("OK");
    } else if (strcmp(cmd, "AT+CGNSINF") == 0) {
        // Mock GPS response
        sim808_mock_queue_response("+CGNSINF: 1,1,20250101,48.117283,11.516667,0.0");
        sim808_mock_queue_response("OK");
    } else if (strncmp(cmd, "AT+CSTT", 7) == 0) {
        sim808_mock_queue_response("OK");
    } else if (strcmp(cmd, "AT+CIICR") == 0) {
        sim808_mock_queue_response("OK");
    } else if (strcmp(cmd, "AT+CIFSR") == 0) {
        sim808_mock_queue_response("192.168.1.100");  // Mock IP
        sim808_mock_queue_response("OK");
    } else {
        sim808_mock_queue_response("OK");
    }
}

void sim808_mock_queue_response(const char *response)
{
    if (!response) return;

    size_t len = strlen(response);
    if (g_resp_idx + len + 2 >= sizeof(g_mock_responses)) return;

    strcpy(&g_mock_responses[g_resp_idx], response);
    g_resp_idx += len;
    g_mock_responses[g_resp_idx++] = '\n';
    g_mock_responses[g_resp_idx] = '\0';
}

int sim808_mock_get_line(char *line, size_t len)
{
    if (!line || len < 2) return 0;
    if (g_resp_idx == 0) return 0;

    // Find newline in buffer
    char *newline = strchr(g_mock_responses, '\n');
    if (!newline) return 0;

    // Extract line
    size_t line_len = newline - g_mock_responses;
    if (line_len >= len) line_len = len - 1;

    strncpy(line, g_mock_responses, line_len);
    line[line_len] = '\0';

    // Remove processed line from buffer
    size_t remaining = g_resp_idx - line_len - 1;
    memmove(g_mock_responses, newline + 1, remaining);
    g_resp_idx = remaining;
    g_mock_responses[g_resp_idx] = '\0';

    return 1;
}

void sim808_mock_task(void)
{
    // Nothing to do in main task
}
