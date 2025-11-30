#include "sim808_driver.h"
#include "sim808_mock.h"
#include <string.h>
#include <stdio.h>

static sim808_state_t g_state = SIM808_STATE_IDLE;
static uint32_t       g_last_tick = 0;
static uint8_t        g_step = 0;

/* Forward‑decl */
static void sim808_process_mock_responses(void);

void sim808_init(void)
{
#if SIM808_USE_MOCK
    sim808_mock_init();
#endif
    g_state = SIM808_STATE_INIT;
    g_last_tick = 0;
    g_step = 0;
}

sim808_state_t sim808_get_state(void)
{
    return g_state;
}

void sim808_task(void)
{
#if SIM808_USE_MOCK
    sim808_mock_task();
    sim808_process_mock_responses();
#endif
}

/* Simple sequence:
   step 0: send "AT"
   step 1: wait OK, then send "AT+CGATT?"
   step 2: wait +CGATT:1 and OK, then send "AT+CGNSPWR=1"
   step 3: wait OK, then consider GPS ON and go to READY
*/
static void sim808_process_mock_responses(void)
{
    static char line[64];

    /* If still initializing, drive the sequence */
    if (g_state == SIM808_STATE_INIT) {
        if (g_step == 0) {
            sim808_mock_send_command("AT");
            g_step = 1;
        }
    }

    while (sim808_mock_get_line(line, sizeof(line))) {
        if (g_step == 1) {
            if (strcmp(line, "OK") == 0) {
                sim808_mock_send_command("AT+CGATT?");
                g_state = SIM808_STATE_GPRS_ATTACH;
                g_step = 2;
            }
        } else if (g_step == 2) {
            if (strncmp(line, "+CGATT: 1", 9) == 0) {
                /* attached, wait for OK */
            } else if (strcmp(line, "OK") == 0) {
                sim808_mock_send_command("AT+CGNSPWR=1");
                g_state = SIM808_STATE_GPS_ON;
                g_step = 3;
            }
        } else if (g_step == 3) {
            if (strcmp(line, "OK") == 0) {
                g_state = SIM808_STATE_READY;
                g_step = 4;
            }
        }
    }
}
