#include "gps_parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ===== PRIVATE HELPERS ===== */

static int parse_rmc_sentence(const char *line, gps_fix_t *fix)
{
    if (!line || !fix) return 0;

    char buf[128];
    strncpy(buf, line, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    // Parse RMC: $GPRMC,time,status,lat,NS,lon,EW,...
    char *tokens[12];
    int count = 0;
    char *ctx = NULL;
    char *tok = strtok_r(buf, ",", &ctx);

    while (tok && count < 12) {
        tokens[count++] = tok;
        tok = strtok_r(NULL, ",", &ctx);
    }

    if (count < 9) return 0;

    // Check status: A = Active (valid fix), V = Void (no fix)
    if (strcmp(tokens[2], "A") != 0) {
        fix->valid_fix = 0;
        return 1;
    }

    // Parse latitude
    double lat = atof(tokens[3]);
    if (lat > 0) {
        lat = (int)(lat / 100) + (lat - (int)(lat / 100) * 100) / 60.0;
        if (strcmp(tokens[4], "S") == 0) lat = -lat;
    }

    // Parse longitude
    double lon = atof(tokens[5]);
    if (lon > 0) {
        lon = (int)(lon / 100) + (lon - (int)(lon / 100) * 100) / 60.0;
        if (strcmp(tokens[6], "W") == 0) lon = -lon;
    }

    fix->valid_fix = 1;
    fix->latitude = lat;
    fix->longitude = lon;

    return 1;
}

/* ===== PUBLIC API ===== */

int gps_parse_nmea(const char *line, gps_fix_t *fix)
{
    if (!line || !fix) return 0;

    memset(fix, 0, sizeof(*fix));

    // Check for RMC sentence
    if (strncmp(line, "$GPRMC", 6) == 0) {
        return parse_rmc_sentence(line, fix);
    }

    // GGA, GSA, and other sentence types could be added here
    return 0;
}
