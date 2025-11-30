#ifndef SIM808_GPS_H
#define SIM808_GPS_H

#include <stdint.h>
#include "gps_parser.h"  /* for gps_fix_t */

/* Initialize SIM808 GPS helper */
void sim808_gps_init(void);

/* Parse a +CGNSINF line and update internal fix.
   Returns 1 if parsed and fix updated, 0 otherwise. */
int sim808_gps_parse_cgnsinf(const char *line);

/* Get latest fix obtained from SIM808.
   Returns 1 if a valid fix is available, 0 otherwise. */
int sim808_gps_get_fix(gps_fix_t *out);

#endif /* SIM808_GPS_H */
