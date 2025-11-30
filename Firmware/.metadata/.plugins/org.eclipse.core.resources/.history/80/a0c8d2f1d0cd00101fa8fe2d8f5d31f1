#ifndef SIM808_DRIVER_H
#define SIM808_DRIVER_H

#include <stdint.h>
#include "sim808_config.h"

/* Simple high-level SIM808 states */
typedef enum {
    SIM808_STATE_IDLE = 0,
    SIM808_STATE_INIT,
    SIM808_STATE_GPRS_ATTACH,
    SIM808_STATE_GPS_ON,
    SIM808_STATE_READY
} sim808_state_t;

/* Result codes for driver operations */
typedef enum {
    SIM808_OK = 0,
    SIM808_ERR_TIMEOUT,
    SIM808_ERR_PARSE,
    SIM808_ERR_HW
} sim808_result_t;

/* Initialize SIM808 driver (mock or real, depending on config) */
void sim808_init(void);

/* Periodic task (call from main loop) */
void sim808_task(void);

/* Get current state */
sim808_state_t sim808_get_state(void);

#endif /* SIM808_DRIVER_H */
