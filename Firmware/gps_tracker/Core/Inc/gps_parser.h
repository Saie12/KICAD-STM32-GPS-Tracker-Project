#ifndef GPS_PARSER_H_
#define GPS_PARSER_H_

#include <stdint.h>

typedef struct {
    int   valid_fix;     /* 1 if GPS has valid fix, 0 otherwise */
    double latitude;     /* decimal degrees, positive = North, negative = South */
    double longitude;    /* decimal degrees, positive = East, negative = West */
} gps_fix_t;

/**
 * @brief Parse a single NMEA line (e.g. $GPRMC,...*CS\r\n).
 *
 * Supports at least GPRMC sentences. If a valid fix is found,
 * 'out' is filled and function returns 1. Otherwise returns 0.
 */
int gps_parse_nmea(const char *line, gps_fix_t *out);

#endif /* GPS_PARSER_H */
