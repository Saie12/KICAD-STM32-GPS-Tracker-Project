#include "stm32f0xx_hal.h"
#include "sim808_power.h"
#include "sim808_driver.h"
#include "sim808_mock.h"
#include <string.h>
#include <stdio.h>

/* Power management state */
static sim808_power_mode_t g_power_mode = SIM808_POWER_ACTIVE;
static sim808_power_mode_t g_target_mode = SIM808_POWER_ACTIVE;
static uint8_t g_watchdog_enabled = 0;
static uint32_t g_watchdog_last_kick = 0;
static uint32_t g_watchdog_timeout_ms = 10000; // 10 seconds

/* Mock battery state */
static sim808_battery_t g_battery_state = {
    .voltage_mv = 3800, // 3.8V
    .charge_percent = 75,
    .charging = 0
};

void sim808_power_init(void)
{
    g_power_mode = SIM808_POWER_ACTIVE;
    g_target_mode = SIM808_POWER_ACTIVE;
    g_watchdog_enabled = 0;
    g_watchdog_last_kick = HAL_GetTick();
}

sim808_result_t sim808_power_set_mode(sim808_power_mode_t mode)
{
    if (mode < SIM808_POWER_ACTIVE || mode > SIM808_POWER_STANDBY) {
        return SIM808_ERR_HW;
    }

    g_target_mode = mode;

#if SIM808_USE_MOCK
    // Mock: transition through power modes
    switch (mode) {
        case SIM808_POWER_ACTIVE:
            sim808_mock_send_command("AT+CFUN=1"); // Full functionality
            break;
        case SIM808_POWER_IDLE:
            sim808_mock_send_command("AT+CSCLK=0"); // Normal mode
            break;
        case SIM808_POWER_SLEEP:
            sim808_mock_send_command("AT+CSCLK=1"); // Sleep mode
            break;
        case SIM808_POWER_STANDBY:
            sim808_mock_send_command("AT+CFUN=0"); // Minimum functionality
            break;
        default:
            return SIM808_ERR_HW;
    }
    g_power_mode = mode;
#endif

    return SIM808_OK;
}

sim808_power_mode_t sim808_power_get_mode(void)
{
    return g_power_mode;
}

sim808_result_t sim808_power_get_battery(sim808_battery_t *bat)
{
    if (!bat) {
        return SIM808_ERR_HW;
    }

    // Mock: simulate battery drain over time
    static uint32_t last_update = 0;
    if (HAL_GetTick() - last_update >= 5000) { // Update every 5 seconds
        last_update = HAL_GetTick();
        // Simulate battery voltage slowly decreasing
        if (g_battery_state.voltage_mv > 3600) {
            g_battery_state.voltage_mv -= 5;
        }
        // Simulate charge percent
        g_battery_state.charge_percent = (g_battery_state.voltage_mv - 3400) / 4;
        if (g_battery_state.charge_percent > 100) {
            g_battery_state.charge_percent = 100;
        }
        if (g_battery_state.charge_percent < 0) {
            g_battery_state.charge_percent = 0;
        }
    }

    *bat = g_battery_state;
    return SIM808_OK;
}

sim808_result_t sim808_power_set_watchdog(uint8_t enable)
{
    g_watchdog_enabled = enable ? 1 : 0;
#if SIM808_USE_MOCK
    if (enable) {
        sim808_mock_send_command("AT+IWDG=1,10"); // Watchdog on, 10 sec timeout
    } else {
        sim808_mock_send_command("AT+IWDG=0"); // Watchdog off
    }
#endif

    if (enable) {
        g_watchdog_last_kick = HAL_GetTick();
    }
    return SIM808_OK;
}

const char *sim808_power_mode_to_str(sim808_power_mode_t mode)
{
    switch (mode) {
        case SIM808_POWER_ACTIVE:   return "ACTIVE";
        case SIM808_POWER_IDLE:     return "IDLE";
        case SIM808_POWER_SLEEP:    return "SLEEP";
        case SIM808_POWER_STANDBY:  return "STANDBY";
        default:                    return "UNKNOWN";
    }
}

void sim808_power_task(void)
{
    // Watchdog kick (periodic reset)
    if (g_watchdog_enabled) {
        if (HAL_GetTick() - g_watchdog_last_kick >= (g_watchdog_timeout_ms / 2)) {
            g_watchdog_last_kick = HAL_GetTick();
            sim808_mock_send_command("AT+IWDG");
        }
    }

    // Battery low check
    sim808_battery_t bat;
    if (sim808_power_get_battery(&bat) == SIM808_OK) {
        if (bat.charge_percent < 10) {
            // Could trigger low battery alert here
        }
    }
}
