#ifndef SIM808_POWER_H
#define SIM808_POWER_H

#include <stdint.h>
#include "sim808_driver.h"

/* Power modes */
typedef enum {
    SIM808_POWER_ACTIVE = 0,
    SIM808_POWER_IDLE,
    SIM808_POWER_SLEEP,
    SIM808_POWER_STANDBY
} sim808_power_mode_t;

/* Battery status */
typedef struct {
    uint16_t voltage_mv;      /* Battery voltage in mV */
    int8_t   charge_percent;  /* Estimated charge 0-100 */
    uint8_t  charging;        /* 1 if charging, 0 otherwise */
} sim808_battery_t;

/* Initialize power management module */
void sim808_power_init(void);

/* Set target power mode (non-blocking) */
sim808_result_t sim808_power_set_mode(sim808_power_mode_t mode);

/* Get current power mode */
sim808_power_mode_t sim808_power_get_mode(void);

/* Get battery status (mock returns fixed values) */
sim808_result_t sim808_power_get_battery(sim808_battery_t *bat);

/* Enable/disable watchdog timer */
sim808_result_t sim808_power_set_watchdog(uint8_t enable);

/* Convert power mode to string for logging */
const char *sim808_power_mode_to_str(sim808_power_mode_t mode);

/* Periodic power management task (call from main loop) */
void sim808_power_task(void);

#endif /* SIM808_POWER_H */
