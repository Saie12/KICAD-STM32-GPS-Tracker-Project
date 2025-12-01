#include "gps_uart.h"
#include "stm32f0xx_hal.h"
#include <string.h>

/* ===== RING BUFFER STATE ===== */

#define GPS_RX_BUFFER_SIZE 256

static uint8_t g_rx_buffer[GPS_RX_BUFFER_SIZE];
static uint16_t g_rx_head = 0;
static uint16_t g_rx_tail = 0;

/* ===== INITIALIZATION ===== */

void gps_uart_init(void)
{
    memset(g_rx_buffer, 0, sizeof(g_rx_buffer));
    g_rx_head = 0;
    g_rx_tail = 0;

    // Start reception interrupt
    HAL_UART_Receive_IT(&huart2, &g_rx_buffer[g_rx_head], 1);
}

/* ===== RX INTERRUPT HANDLER ===== */

void gps_uart_on_rx_complete(void)
{
    // Move head pointer
    g_rx_head = (g_rx_head + 1) % GPS_RX_BUFFER_SIZE;

    // Restart reception for next byte
    HAL_UART_Receive_IT(&huart2, &g_rx_buffer[g_rx_head], 1);
}

/* ===== LINE EXTRACTION ===== */

int gps_uart_get_line(char *line, size_t len)
{
    if (!line || len < 2) return 0;

    // Check if data is available
    if (g_rx_tail == g_rx_head) {
        return 0;  // No data
    }

    // Extract until newline or buffer full
    size_t i = 0;
    while (g_rx_tail != g_rx_head && i < len - 1) {
        uint8_t c = g_rx_buffer[g_rx_tail];
        g_rx_tail = (g_rx_tail + 1) % GPS_RX_BUFFER_SIZE;

        if (c == '\n') {
            // End of line found
            line[i] = '\0';
            return 1;
        }

        if (c >= 32 && c <= 126) {
            // Printable ASCII
            line[i++] = c;
        }
    }

    return 0;  // No complete line yet
}
