#ifndef SCREEN_GAMEPLAY_H
#define SCREEN_GAMEPLAY_H

#include <string.h>

#include "raylib.h"
#include "raymath.h"

#include "types.h"
#include "../wave.h"
#include "../update/glyph.h"
#include "../update/player.h"
#include "../update/enemy.h"
#include "../update/bullet.h"

#define N_WAVES 20
#define TIME_LIMIT 300

typedef struct {
    Texture2D background;
    Texture2D tiles_sprite;
    Texture2D player_sprite;
    Texture2D signs_sprite;
    Texture2D enemies_sprite;
    Texture2D bullets_sprite;
    Texture2D status_sprite;
    Music music;
} GameplayAssets;

typedef struct {
    float tick;
    Grid grid;
    Player player;
    GlyphSpawner glyph_spawner;
    Glyph glyphs[MAX_GLYPHS];
    Enemy enemies[MAX_ENEMIES];
    Bullet bullets[MAX_BULLETS];
    uint8_t wave;
    uint8_t countdown;
    uint16_t time_limit;
    bool is_clear;
    size_t target_enemy;
} GameplayState;

void load_gameplay_assets(GameplayAssets *a);

void unload_gameplay_assets(GameplayAssets *a);

void load_gameplay_screen(GameplayState *s, const GameplayAssets *a);

void unload_gameplay_screen(GameplayState *s, const GameplayAssets *a);

void update_gameplay(GameplayState *s, const Sounds *sounds, Screen *next_screen);

void update_gameplay_music(const GameplayAssets *a);

void draw_gameplay(const GameplayState *s, const GameplayAssets *a);

#endif
