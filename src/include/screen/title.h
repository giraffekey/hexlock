#ifndef SCREEN_TITLE_H
#define SCREEN_TITLE_H

#include "raylib.h"

#include "types.h"

typedef struct {
    Texture2D background;
    Music music;
} TitleAssets;

typedef struct {
    float t;
    bool is_increasing;
} TitleState;

void load_title_assets(TitleAssets *a);

void unload_title_assets(TitleAssets *a);

void load_title_screen(TitleState *s, const TitleAssets *a);

void unload_title_screen(TitleState *s, const TitleAssets *a);

void update_title(TitleState *s, Screen *next_screen);

void update_title_music(const TitleAssets *a);

void draw_title(const TitleState *s, const TitleAssets *a);

#endif
