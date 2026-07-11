#ifndef UPDATE_GLYPH_H
#define UPDATE_GLYPH_H

#include "raylib.h"

#include "../glyph.h"
#include "../player.h"

void update_glyphs(Glyph glyphs[]);

void update_glyph_spawner(GlyphSpawner *spawner, Glyph glyphs[], uint8_t wave, const Grid grid, const Player *player);

void update_glyphs_time(Glyph glyphs[], float dt);

#endif
