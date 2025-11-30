#ifndef SIM808_MOCK_H
#define SIM808_MOCK_H

#include <stdint.h>

/* Called by driver to initialize mock module */
void sim808_mock_init(void);

/* Called by driver when it "sends" an AT command */
void sim808_mock_send_command(const char *cmd);

/* Called periodically by driver to let mock update internal state */
void sim808_mock_task(void);

/* Called by driver to retrieve the latest response line (if any).
   Returns 1 if a line is copied into out, 0 if no new line. */
int sim808_mock_get_line(char *out, int max_len);

#endif /* SIM808_MOCK_H */
