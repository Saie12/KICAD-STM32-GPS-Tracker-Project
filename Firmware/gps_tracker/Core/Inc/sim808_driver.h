/**
 * @file sim808_driver.h
 * @brief SIM808 GSM/GPS module driver header
 *        Core state machine and AT command API
 */

#ifndef SIM808_DRIVER_H
#define SIM808_DRIVER_H

#include <stdint.h>

/* ===== TYPES & ENUMERATIONS ===== */

typedef enum {
    SIM808_OK = 0,
    SIM808_ERR_HW = 1
} sim808_result_t;

typedef enum {
    SIM808_STATE_IDLE = 0,
    SIM808_STATE_INIT,
    SIM808_STATE_GPRS_ATTACH,
    SIM808_STATE_GPS_ON,
    SIM808_STATE_READY
} sim808_state_t;

typedef enum {
    SIM808_CMD_NONE = 0,
    SIM808_CMD_GET_GPS_INFO,
    SIM808_CMD_GPRS_ATTACH,
    SIM808_CMD_TCP_CONNECT,
    SIM808_CMD_TCP_SEND
} sim808_cmd_t;

typedef enum {
    SIM808_ERR_NONE = 0,
    SIM808_ERR_TIMEOUT_OP,
    SIM808_ERR_CMD_REJECTED,
    SIM808_ERR_NOT_READY
} sim808_error_t;

typedef struct {
    sim808_error_t last_error;
    uint32_t       error_count;
} sim808_error_status_t;

/* ===== PUBLIC API ===== */

void sim808_init(void);

sim808_state_t sim808_get_state(void);

const char *sim808_state_to_str(sim808_state_t st);

sim808_result_t sim808_request_gps_info(void);

void sim808_task(void);

void sim808_get_error_status(sim808_error_status_t *st);

#endif /* SIM808_DRIVER_H */
