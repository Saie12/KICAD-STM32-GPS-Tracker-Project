#include "stm32f0xx_hal.h"
#include "sim808_driver.h"
#include "sim808_mock.h"
#include "sim808_gps.h"
#include <string.h>
#include <stdio.h>

/* ===== STATE MACHINE & ERROR TRACKING ===== */

static sim808_state_t   g_state          = SIM808_STATE_IDLE;
static uint8_t          g_step           = 0;
static sim808_cmd_t     g_active_cmd     = SIM808_CMD_NONE;
static sim808_error_status_t g_err       = { SIM808_ERR_NONE, 0 };
static uint32_t         g_cmd_start_tick = 0;
static const uint32_t   g_cmd_timeout_ms = 3000U;

/* ===== PRIVATE FUNCTIONS ===== */

static void sim808_process_mock_responses(void);

static void sim808_set_error(sim808_error_t e)
{
    g_err.last_error = e;
    g_err.error_count++;
}

/* ===== PUBLIC API ===== */

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

void sim808_get_error_status(sim808_error_status_t *st)
{
    if (!st) return;
    *st = g_err;
}

void sim808_init(void)
{
    // Reset error tracking
    g_err.last_error  = SIM808_ERR_NONE;
    g_err.error_count = 0;

    // Reset command state
    g_active_cmd     = SIM808_CMD_NONE;
    g_cmd_start_tick = 0;

    // Initialize GPS helper
    sim808_gps_init();

#if SIM808_USE_MOCK
    sim808_mock_init();
#endif

    g_state = SIM808_STATE_INIT;
    g_step  = 0;
}

sim808_state_t sim808_get_state(void)
{
    return g_state;
}

sim808_result_t sim808_request_gps_info(void)
{
    // Must be in READY state and not busy
    if (g_state != SIM808_STATE_READY) {
        sim808_set_error(SIM808_ERR_NOT_READY);
        return SIM808_ERR_HW;
    }

    if (g_active_cmd != SIM808_CMD_NONE) {
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

void sim808_task(void)
{
#if SIM808_USE_MOCK
    sim808_mock_task();
    sim808_process_mock_responses();

    // Check for command timeout
    if (g_active_cmd != SIM808_CMD_NONE) {
        uint32_t now = HAL_GetTick();
        if ((now - g_cmd_start_tick) > g_cmd_timeout_ms) {
            sim808_set_error(SIM808_ERR_TIMEOUT_OP);
            g_active_cmd = SIM808_CMD_NONE;
        }
    }
#endif
}

/* ===== STATE MACHINE HANDLER ===== */

static void sim808_process_mock_responses(void)
{
    char line[64];

    // Kick off initialization sequence
    if (g_state == SIM808_STATE_INIT && g_step == 0) {
        sim808_mock_send_command("AT");
        g_step = 1;
    }

    // Process responses from mock layer
    while (sim808_mock_get_line(line, sizeof(line))) {

        // --- Initialization steps ---
        if (g_step == 1) {
            if (strcmp(line, "OK") == 0) {
                sim808_mock_send_command("AT+CGATT?");
                g_state = SIM808_STATE_GPRS_ATTACH;
                g_step = 2;
            }
        } else if (g_step == 2) {
            if (strncmp(line, "+CGATT: 1", 9) == 0) {
                // Attached, wait for OK
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

        // --- Command handling (READY state) ---
        if (g_state == SIM808_STATE_READY && g_active_cmd != SIM808_CMD_NONE) {

            if (g_active_cmd == SIM808_CMD_GET_GPS_INFO) {
                if (strncmp(line, "+CGNSINF:", 9) == 0) {
                    // Parse GPS data from SIM808
                    sim808_gps_parse_cgnsinf(line);
                } else if (strcmp(line, "OK") == 0) {
                    // Command completed
                    g_active_cmd = SIM808_CMD_NONE;
                } else if (strcmp(line, "ERROR") == 0) {
                    // Command failed
                    sim808_set_error(SIM808_ERR_CMD_REJECTED);
                    g_active_cmd = SIM808_CMD_NONE;
                }
            }
        }
    }
}
