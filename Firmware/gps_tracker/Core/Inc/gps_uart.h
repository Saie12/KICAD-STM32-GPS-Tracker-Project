#ifndef GPS_UART_H_
#define GPS_UART_H_

#include <stdint.h>
#include "gps_parser.h"  /* for gps_fix_t if needed */

#define GPS_RX_BUFFER_SIZE 256
#define GPS_LINE_BUFFER_SIZE 128

void gps_uart_init(void);                 /* start UART2 RX IT */
void gps_uart_rx_byte(uint8_t byte);      /* called from ISR */
int  gps_uart_get_line(char *out, int max_len); /* returns 1 if full line copied */
void gps_uart_on_rx_complete(void);

#endif /* GPS_UART_H */
