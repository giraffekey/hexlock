#ifndef SCREEN_DEATH_H
#define SCREEN_DEATH_H

#include "raylib.h"

#include "types.h"

typedef struct {
    Texture2D background;
} DeathAssets;

typedef struct {} DeathState;

void load_death_assets(DeathAssets *a);

void unload_death_assets(DeathAssets *a);

void load_death_screen(DeathState *s);

void update_death(DeathState *s, Screen *next_screen);

void draw_death(const DeathState *s, const DeathAssets *a);

#endif
