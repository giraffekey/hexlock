#ifndef SOUNDS_H
#define SOUNDS_H

#include "raylib.h"

typedef struct {
	Sound magic;
	Sound special;
	Sound throw;
	Sound hit;
	Sound block;
	Sound heal;
	Sound reflect;
	Sound power;
	Sound advance;
	Sound glyph;
	Sound success;
	Sound fail;
	Sound place;
	Sound charge;
	Sound teleport;
	Sound win;
	Sound death;
} Sounds;

void load_sounds(Sounds *sounds);

void unload_sounds(Sounds *sounds);

#endif
