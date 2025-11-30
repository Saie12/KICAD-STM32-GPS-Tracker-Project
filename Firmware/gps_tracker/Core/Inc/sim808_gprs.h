#ifndef SIM808_GPRS_H
#define SIM808_GPRS_H

#include <stdint.h>
#include "sim808_driver.h"

/* GPRS connection states */
typedef enum {
    SIM808_GPRS_DISCONNECTED = 0,
    SIM808_GPRS_CONNECTING,
    SIM808_GPRS_CONNECTED
} sim808_gprs_state_t;

/* Initialize GPRS module */
void sim808_gprs_init(void);

/* Get current GPRS state */
sim808_gprs_state_t sim808_gprs_get_state(void);

/* Attach to GPRS network with APN.
   Returns SIM808_OK if initiated, SIM808_ERR_HW if not ready. */
sim808_result_t sim808_gprs_attach(const char *apn);

/* Open TCP connection to host:port.
   Returns SIM808_OK if initiated, SIM808_ERR_HW if not ready. */
sim808_result_t sim808_gprs_open_tcp(const char *host, uint16_t port);

/* Send payload over open TCP connection.
   Returns SIM808_OK if initiated, SIM808_ERR_HW if not ready. */
sim808_result_t sim808_gprs_send_data(const char *payload);

/* Close TCP connection */
sim808_result_t sim808_gprs_close(void);

/* Convert GPRS state to string for logging */
const char *sim808_gprs_state_to_str(sim808_gprs_state_t state);

#endif /* SIM808_GPRS_H */
