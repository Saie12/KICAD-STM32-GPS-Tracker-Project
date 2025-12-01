/**
 * @file sim808_gps.h
 * @brief SIM808 GPS data parsing (CGNSINF format)
 */

#ifndef SIM808_GPS_H
#define SIM808_GPS_H

#include <stdint.h>
#include "gps_parser.h"

/* ===== PUBLIC API ===== */

void sim808_gps_init(void);

int sim808_gps_parse_cgnsinf(const char *line);

int sim808_gps_get_fix(gps_fix_t *out);

#endif /* SIM808_GPS_H */
