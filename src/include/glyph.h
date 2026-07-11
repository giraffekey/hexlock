#ifndef GLYPH_H
#define GLYPH_H

#include "util.h"

#define MAX_SIGNS 3
#define MAX_GLYPHS 3
#define GLYPH_SPAWN_RATE 20

typedef enum {
    ARIES,
    TAURUS,
    GEMINI,
    CANCER,
    LEO,
    VIRGO,
    LIBRA,
    SCORPIO,
    SAGITTARIUS,
    CAPRICORN,
    AQUARIUS,
    PISCES,
    N_SIGNS,
} Sign;

typedef uint8_t SignU8;

typedef struct {
    SignU8 chunk[6];
    uint8_t cooldown;
    size_t index;
} GlyphSpawner;

typedef struct {
    bool exists;
    Position pos;
    float t;
    SignU8 sign;
    uint8_t lifetime;
} Glyph;

void generate_glyph_chunk(GlyphSpawner *spawner);

#endif
