/**
 * @file gps_uart.h
 * @brief GPS module UART interface with ring buffering
 */

#ifndef GPS_UART_H
#define GPS_UART_H

#include <stddef.h>
#include "main.h"

#define GPS_LINE_BUFFER_SIZE 128

/* ===== PUBLIC API ===== */

void gps_uart_init(void);

void gps_uart_on_rx_complete(void);

int gps_uart_get_line(char *line, size_t len);

#endif /* GPS_UART_H */
