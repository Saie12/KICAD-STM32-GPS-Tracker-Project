/**
 * @file sim808_mock.h
 * @brief SIM808 mock layer for hardware-free testing
 */

#ifndef SIM808_MOCK_H
#define SIM808_MOCK_H

#include <stddef.h>

/* ===== PUBLIC API ===== */

void sim808_mock_init(void);

void sim808_mock_send_command(const char *cmd);

void sim808_mock_queue_response(const char *response);

int sim808_mock_get_line(char *line, size_t len);

void sim808_mock_task(void);

#endif /* SIM808_MOCK_H */
