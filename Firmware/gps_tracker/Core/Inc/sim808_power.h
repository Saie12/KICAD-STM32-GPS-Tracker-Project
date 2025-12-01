/**
 * @file sim808_power.h
 * @brief SIM808 power management and battery monitoring
 */

#ifndef SIM808_POWER_H
#define SIM808_POWER_H

#include <stdint.h>
#include "sim808_driver.h"

/* ===== POWER MODES ===== */

typedef enum {
    SIM808_POWER_ACTIVE = 0,
    SIM808_POWER_IDLE,
    SIM808_POWER_SLEEP,
    SIM808_POWER_STANDBY
} sim808_power_mode_t;

/* ===== BATTERY STATE ===== */

typedef struct {
    uint16_t voltage_mv;        // Battery voltage in mV
    int16_t  charge_percent;    // Battery charge percentage (0-100)
    uint8_t  charging;          // 1 = charging, 0 = not charging
} sim808_battery_t;

/* ===== PUBLIC API ===== */

void sim808_power_init(void);

sim808_result_t sim808_power_set_mode(sim808_power_mode_t mode);

sim808_power_mode_t sim808_power_get_mode(void);

sim808_result_t sim808_power_get_battery(sim808_battery_t *bat);

sim808_result_t sim808_power_set_watchdog(uint8_t enable);

const char *sim808_power_mode_to_str(sim808_power_mode_t mode);

void sim808_power_task(void);

#endif /* SIM808_POWER_H */
