/**
 * @file gps_parser.h
 * @brief NMEA GPS sentence parser (RMC, GGA)
 */

#ifndef GPS_PARSER_H
#define GPS_PARSER_H

#include <stdint.h>

/* ===== GPS FIX STRUCTURE ===== */

typedef struct {
    uint8_t  valid_fix;     // 1 = valid fix, 0 = no fix
    double   latitude;      // Decimal degrees
    double   longitude;     // Decimal degrees
    double   altitude;      // Meters (if available)
    double   speed;         // Knots (if available)
    uint32_t timestamp;     // Unix timestamp (if available)
} gps_fix_t;

/* ===== PUBLIC API ===== */

int gps_parse_nmea(const char *line, gps_fix_t *fix);

#endif /* GPS_PARSER_H */
