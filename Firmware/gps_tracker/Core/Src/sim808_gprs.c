#include "sim808_gprs.h"
#include "sim808_mock.h"
#include <string.h>
#include <stdio.h>

/* ===== GPRS STATE ===== */

static sim808_gprs_state_t g_gprs_state = SIM808_GPRS_DISCONNECTED;
static uint32_t g_gprs_connect_tick = 0;

/* ===== PUBLIC API ===== */

void sim808_gprs_init(void)
{
    g_gprs_state = SIM808_GPRS_DISCONNECTED;
    g_gprs_connect_tick = 0;
}

sim808_gprs_state_t sim808_gprs_get_state(void)
{
    return g_gprs_state;
}

sim808_result_t sim808_gprs_attach(const char *apn)
{
    if (!apn) {
        return SIM808_ERR_HW;
    }

    if (sim808_get_state() != SIM808_STATE_READY) {
        return SIM808_ERR_HW;
    }

    // If already connected, no need to attach again
    if (g_gprs_state == SIM808_GPRS_CONNECTED) {
        return SIM808_OK;
    }

    // Transition to CONNECTING
    g_gprs_state = SIM808_GPRS_CONNECTING;
    g_gprs_connect_tick = HAL_GetTick();

#if SIM808_USE_MOCK
    // Simulate GPRS attach sequence
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CSTT=\"%s\"", apn);
    sim808_mock_send_command(cmd);
    sim808_mock_send_command("AT+CIICR");
    sim808_mock_send_command("AT+CIFSR");

    // Mock: immediately mark as connected
    g_gprs_state = SIM808_GPRS_CONNECTED;
#endif

    return SIM808_OK;
}

sim808_result_t sim808_gprs_open_tcp(const char *host, uint16_t port)
{
    if (!host) {
        return SIM808_ERR_HW;
    }

    if (g_gprs_state != SIM808_GPRS_CONNECTED) {
        return SIM808_ERR_HW;
    }

#if SIM808_USE_MOCK
    // Simulate TCP connection
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",%u", host, port);
    sim808_mock_send_command(cmd);
    sim808_mock_send_command("OK");
#endif

    return SIM808_OK;
}

sim808_result_t sim808_gprs_send_data(const char *payload)
{
    if (!payload) {
        return SIM808_ERR_HW;
    }

    if (g_gprs_state != SIM808_GPRS_CONNECTED) {
        return SIM808_ERR_HW;
    }

#if SIM808_USE_MOCK
    // Simulate data transmission
    sim808_mock_send_command("AT+CIPSEND");

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "%s", payload);
    sim808_mock_send_command(cmd);
    sim808_mock_send_command("OK");
#endif

    return SIM808_OK;
}

sim808_result_t sim808_gprs_close(void)
{
#if SIM808_USE_MOCK
    sim808_mock_send_command("AT+CIPCLOSE=1");
    sim808_mock_send_command("OK");
#endif

    g_gprs_state = SIM808_GPRS_DISCONNECTED;
    return SIM808_OK;
}

const char *sim808_gprs_state_to_str(sim808_gprs_state_t state)
{
    switch (state) {
    case SIM808_GPRS_DISCONNECTED: return "DISCONNECTED";
    case SIM808_GPRS_CONNECTING:   return "CONNECTING";
    case SIM808_GPRS_CONNECTED:    return "CONNECTED";
    default:                       return "UNKNOWN";
    }
}
