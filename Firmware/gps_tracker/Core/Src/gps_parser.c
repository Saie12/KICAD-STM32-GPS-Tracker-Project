#include "gps_parser.h"
#include <string.h>
#include <stdlib.h>

/* Helper: convert ddmm.mmmm (NMEA) + N/S or E/W to decimal degrees */
static double nmea_to_decimal(const char *field, char hemi)
{
    if (field == NULL || *field == '\0') {
        return 0.0;
    }

    /* Find decimal point to split degrees and minutes */
    const char *dot = strchr(field, '.');
    if (dot == NULL || dot - field < 2) {
        return 0.0;
    }

    int deg_digits = (int)(dot - field) - 2;
    if (deg_digits <= 0) {
        return 0.0;
    }

    char deg_str[4] = {0};
    char min_str[16] = {0};

    memcpy(deg_str, field, (size_t)deg_digits);
    strcpy(min_str, field + deg_digits);

    double deg = atof(deg_str);
    double minutes = atof(min_str);

    double dec = deg + minutes / 60.0;

    if (hemi == 'S' || hemi == 's' || hemi == 'W' || hemi == 'w') {
        dec = -dec;
    }

    return dec;
}

/* Split line into comma‑separated fields in-place */
static int split_fields(char *line, char *fields[], int max_fields)
{
    int count = 0;
    char *p = line;

    if (*p == '$') {
        p++; /* skip leading $ */
    }

    fields[count++] = p;

    while (*p != '\0' && count < max_fields) {
        if (*p == ',') {
            *p = '\0';
            fields[count++] = p + 1;
        } else if (*p == '*') {
            *p = '\0';
            break;
        }
        p++;
    }

    return count;
}

int gps_parse_nmea(const char *line_in, gps_fix_t *out)
{
    if (line_in == NULL || out == NULL) {
        return 0;
    }

    /* Make a local modifiable copy */
    char buf[128];
    size_t len = strlen(line_in);
    if (len >= sizeof(buf)) {
        return 0;
    }
    strcpy(buf, line_in);

    /* Quick check for GPRMC */
    if (strstr(buf, "GPRMC,") != buf + 1) {
        /* Not a $GPRMC sentence */
        return 0;
    }

    /* Split into fields */
    char *fields[16] = {0};
    int n = split_fields(buf, fields, 16);
    if (n < 10) {
        return 0;
    }

    /*
       GPRMC format (fields indices after $GPRMC):
       0: "GPRMC"
       1: time (hhmmss.sss)
       2: status (A=valid, V=nav warning)
       3: latitude
       4: N/S
       5: longitude
       6: E/W
       ... (we only need 2,3,4,5,6 for now)
    */

    const char status = (fields[2] && fields[2][0]) ? fields[2][0] : 'V';
    if (status != 'A') {
        out->valid_fix = 0;
        return 1; /* Parsed but no valid fix */
    }

    const char *lat_str = fields[3];
    const char *lat_hemi_str = fields[4];
    const char *lon_str = fields[5];
    const char *lon_hemi_str = fields[6];

    if (!lat_str || !lat_hemi_str || !lon_str || !lon_hemi_str) {
        return 0;
    }

    char lat_hemi = lat_hemi_str[0];
    char lon_hemi = lon_hemi_str[0];

    double lat = nmea_to_decimal(lat_str, lat_hemi);
    double lon = nmea_to_decimal(lon_str, lon_hemi);

    out->valid_fix = 1;
    out->latitude  = lat;
    out->longitude = lon;

    return 1;
}
