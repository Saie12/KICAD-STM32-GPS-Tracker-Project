#include "sim808_driver.h"
#include "sim808_mock.h"
#include "sim808_gps.h"
#include <string.h>
#include <stdio.h>

/* Global SIM808 state */
static sim808_state_t g_state = SIM808_STATE_IDLE;
static uint8_t        g_step  = 0;

/* Current active command and error status */
static sim808_cmd_t           g_active_cmd     = SIM808_CMD_NONE;
static sim808_error_status_t  g_err            = { SIM808_ERR_NONE, 0 };
static uint32_t               g_cmd_start_tick = 0;
static const uint32_t         g_cmd_timeout_ms = 3000U; /* 3s per command */

/* Forward declarations */
static void sim808_process_mock_responses(void);
static void sim808_set_error(sim808_error_t e);

/* Convert state to string */
const char *sim808_state_to_str(sim808_state_t st)
{
    switch (st) {
    case SIM808_STATE_IDLE:        return "IDLE";
    case SIM808_STATE_INIT:        return "INIT";
    case SIM808_STATE_GPRS_ATTACH: return "GPRS_ATTACH";
    case SIM808_STATE_GPS_ON:      return "GPS_ON";
    case SIM808_STATE_READY:       return "READY";
    default:                       return "UNKNOWN";
    }
}

/* Internal: record error */
static void sim808_set_error(sim808_error_t e)
{
    g_err.last_error = e;
    g_err.error_count++;
}

/* Public: get last error + count */
void sim808_get_error_status(sim808_error_status_t *st)
{
    if (!st) return;
    *st = g_err;
}

/* Initialize SIM808 core driver */
void sim808_init(void)
{
    /* Reset error status */
    g_err.last_error  = SIM808_ERR_NONE;
    g_err.error_count = 0;

    /* Reset command tracking */
    g_active_cmd     = SIM808_CMD_NONE;
    g_cmd_start_tick = 0;

    /* Initialize GPS helper (for CGNSINF parsing) */
    sim808_gps_init();

#if SIM808_USE_MOCK
    sim808_mock_init();
#endif

    g_state = SIM808_STATE_INIT;
    g_step  = 0;
}

/* Get current high‑level state */
sim808_state_t sim808_get_state(void)
{
    return g_state;
}

/* Request GPS info once (AT+CGNSINF) – non‑blocking */
sim808_result_t sim808_request_gps_info(void)
{
    /* Must be ready and not already busy with another command */
    if (g_state != SIM808_STATE_READY) {
        sim808_set_error(SIM808_ERR_NOT_READY);
        return SIM808_ERR_HW;
    }
    if (g_active_cmd != SIM808_CMD_NONE) {
        /* Busy with another command */
        sim808_set_error(SIM808_ERR_NOT_READY);
        return SIM808_ERR_HW;
    }

#if SIM808_USE_MOCK
    sim808_mock_send_command("AT+CGNSINF");
#endif

    g_active_cmd     = SIM808_CMD_GET_GPS_INFO;
    g_cmd_start_tick = HAL_GetTick();
    return SIM808_OK;
}

/* Periodic task: drive mock + check timeouts */
void sim808_task(void)
{
#if SIM808_USE_MOCK
    sim808_mock_task();
    sim808_process_mock_responses();

    /* Command timeout check */
    if (g_active_cmd != SIM808_CMD_NONE) {
        uint32_t now = HAL_GetTick();
        if ((now - g_cmd_start_tick) > g_cmd_timeout_ms) {
            sim808_set_error(SIM808_ERR_TIMEOUT_OP);
            g_active_cmd = SIM808_CMD_NONE;
        }
    }
#endif
}

/*
 * Initialization sequence with mock:
 *   step 0: send "AT"
 *   step 1: wait OK, then send "AT+CGATT?"
 *   step 2: wait +CGATT:1 and OK, then send "AT+CGNSPWR=1"
 *   step 3: wait OK, then consider GPS ON and go to READY
 *
 * Also handles responses for active commands when READY,
 * currently only SIM808_CMD_GET_GPS_INFO (AT+CGNSINF).
 */
static void sim808_process_mock_responses(void)
{
    char line[64];

    /* Kick off init sequence */
    if (g_state == SIM808_STATE_INIT && g_step == 0) {
        sim808_mock_send_command("AT");
        g_step = 1;
    }

    while (sim808_mock_get_line(line, sizeof(line))) {

        /* ----- Init state machine ----- */
        if (g_step == 1) {
            if (strcmp(line, "OK") == 0) {
                sim808_mock_send_command("AT+CGATT?");
                g_state = SIM808_STATE_GPRS_ATTACH;
                g_step  = 2;
            }
        } else if (g_step == 2) {
            if (strncmp(line, "+CGATT: 1", 9) == 0) {
                /* attached, wait for trailing OK */
            } else if (strcmp(line, "OK") == 0) {
                sim808_mock_send_command("AT+CGNSPWR=1");
                g_state = SIM808_STATE_GPS_ON;
                g_step  = 3;
            }
        } else if (g_step == 3) {
            if (strcmp(line, "OK") == 0) {
                g_state = SIM808_STATE_READY;
                g_step  = 4;
            }
        }

        /* ----- Command handling once module is READY ----- */
        if (g_state == SIM808_STATE_READY && g_active_cmd != SIM808_CMD_NONE) {

            if (g_active_cmd == SIM808_CMD_GET_GPS_INFO) {

                if (strncmp(line, "+CGNSINF:", 9) == 0) {
                    /* Parse GPS info from SIM808 */
                    sim808_gps_parse_cgnsinf(line);
                } else if (strcmp(line, "OK") == 0) {
                    /* Command completed successfully */
                    g_active_cmd = SIM808_CMD_NONE;
                } else if (strcmp(line, "ERROR") == 0) {
                    /* Command rejected */
                    sim808_set_error(SIM808_ERR_CMD_REJECTED);
                    g_active_cmd = SIM808_CMD_NONE;
                }
            }

            /* Future commands (GPRS/TCP etc.) can be handled here using g_active_cmd */
        }
    }
}
