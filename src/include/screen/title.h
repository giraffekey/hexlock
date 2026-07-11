#ifndef SCREEN_TITLE_H
#define SCREEN_TITLE_H

#include "raylib.h"

#include "types.h"

typedef struct {
    Texture2D background;
} TitleAssets;

typedef struct {} TitleState;

void load_title_assets(TitleAssets *a);

void unload_title_assets(TitleAssets *a);

void load_title_screen(TitleState *s);

void update_title(TitleState *s, Screen *next_screen);

void draw_title(const TitleState *s, const TitleAssets *a);

#endif
