#ifndef UPDATE_GLYPH_H
#define UPDATE_GLYPH_H

#include "raylib.h"

#include "../glyph.h"
#include "../player.h"

void spawn_glyph(SignU8 sign, Glyph glyphs[], const Grid grid, const Player *player);

void update_glyphs(Glyph glyphs[]);

void update_glyph_spawner(GlyphSpawner *spawner, Glyph glyphs[], const Grid grid, const Player *player);

void update_glyphs_time(Glyph glyphs[], float dt);

#endif
