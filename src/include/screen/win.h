#ifndef SCREEN_WIN_H
#define SCREEN_WIN_H

#include <stdint.h>

#include "raylib.h"

#include "types.h"
#include "../storage.h"

typedef struct {
    Texture2D background;
} WinAssets;

typedef struct {
    uint16_t score;
    uint16_t high_score;
} WinState;

void load_win_assets(WinAssets *a);

void unload_win_assets(WinAssets *a);

void load_win_screen(WinState *s);

void unload_win_screen(WinState *s);

void update_win(WinState *s, Screen *next_screen);

void draw_win(const WinState *s, const WinAssets *a);

#endif
