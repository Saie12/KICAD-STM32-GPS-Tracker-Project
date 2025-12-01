#include "sim808_gps.h"
#include <string.h>
#include <stdlib.h>

/* ===== INTERNAL STATE ===== */

static gps_fix_t g_sim808_fix;

/* ===== HELPERS ===== */

static int parse_float(const char *s, double *out)
{
    if (!s || !out || !*s) return 0;

    char *endptr = NULL;
    double v = strtod(s, &endptr);

    if (endptr == s) return 0;

    *out = v;
    return 1;
}

/* ===== PUBLIC API ===== */

void sim808_gps_init(void)
{
    memset(&g_sim808_fix, 0, sizeof(g_sim808_fix));
    g_sim808_fix.valid_fix = 0;
}

/*
 * Parse +CGNSINF response from SIM808
 * Format: +CGNSINF: <run>,<fix>,<utc>,<lat>,<lon>,...
 */
int sim808_gps_parse_cgnsinf(const char *line)
{
    if (!line) return 0;

    const char *p = strstr(line, "+CGNSINF:");
    if (!p) return 0;

    // Skip past the "+CGNSINF:" marker
    p += 9;
    if (*p == ' ') p++;

    // Copy to local buffer for tokenization
    char buf[128];
    strncpy(buf, p, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    // Parse comma-separated fields
    char *tokens[12];
    int count = 0;
    char *ctx = NULL;

    char *tok = strtok_r(buf, ",", &ctx);
    while (tok && count < 12) {
        tokens[count++] = tok;
        tok = strtok_r(NULL, ",", &ctx);
    }

    if (count < 5) return 0;

    int run_status = atoi(tokens[0]);
    int fix_status = atoi(tokens[1]);

    gps_fix_t fix;
    memset(&fix, 0, sizeof(fix));

    // Check if we have a valid fix
    if (run_status == 1 && (fix_status == 1 || fix_status == 2)) {
        double lat = 0.0, lon = 0.0;

        if (!parse_float(tokens[3], &lat)) return 0;
        if (!parse_float(tokens[4], &lon)) return 0;

        fix.valid_fix = 1;
        fix.latitude  = lat;
        fix.longitude = lon;
    } else {
        fix.valid_fix = 0;
    }

    g_sim808_fix = fix;
    return 1;
}

int sim808_gps_get_fix(gps_fix_t *out)
{
    if (!out) return 0;

    *out = g_sim808_fix;
    return g_sim808_fix.valid_fix ? 1 : 0;
}
