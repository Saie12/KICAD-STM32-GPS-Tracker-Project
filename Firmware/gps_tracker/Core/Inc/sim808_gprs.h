/**
 * @file sim808_gprs.h
 * @brief SIM808 GPRS and TCP data transmission
 */

#ifndef SIM808_GPRS_H
#define SIM808_GPRS_H

#include <stdint.h>
#include "sim808_driver.h"

/* ===== GPRS STATES ===== */

typedef enum {
    SIM808_GPRS_DISCONNECTED = 0,
    SIM808_GPRS_CONNECTING,
    SIM808_GPRS_CONNECTED
} sim808_gprs_state_t;

/* ===== PUBLIC API ===== */

void sim808_gprs_init(void);

sim808_gprs_state_t sim808_gprs_get_state(void);

sim808_result_t sim808_gprs_attach(const char *apn);

sim808_result_t sim808_gprs_open_tcp(const char *host, uint16_t port);

sim808_result_t sim808_gprs_send_data(const char *payload);

sim808_result_t sim808_gprs_close(void);

const char *sim808_gprs_state_to_str(sim808_gprs_state_t state);

#endif /* SIM808_GPRS_H */
