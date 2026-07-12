/*******************************************************************************************
*
*   raylib gamejam hex + merge
*
*   Code licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2026 giraffekey
*
********************************************************************************************/

#include <stdio.h>
#include <time.h>

#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#include "include/util.h"
#include "include/glyph.h"
#include "include/player.h"
#include "include/enemy.h"
#include "include/bullet.h"
#include "include/collide.h"
#include "include/wave.h"
#include "include/sounds.h"
#include "include/update/glyph.h"
#include "include/update/player.h"
#include "include/update/enemy.h"
#include "include/update/bullet.h"
#include "include/screen/title.h"
#include "include/screen/gameplay.h"
#include "include/screen/win.h"
#include "include/screen/death.h"

#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720
#define GAME_WIDTH 180
#define GAME_HEIGHT 180

static const Color CLEAR_COLOR = {30, 29, 57, 255};

typedef struct {
    TitleAssets title;
    GameplayAssets gameplay;
    WinAssets win;
    DeathAssets death;
    Sounds sounds;
} Assets;

typedef union {
    TitleState title;
    GameplayState gameplay;
    WinState win;
    DeathState death;
} ScreenState;

typedef struct {
    RenderTexture2D target;
    Assets assets;
    Screen screen;
    ScreenState screen_state;
} State;

void load_assets(Assets *a) {
    load_title_assets(&a->title);
    load_gameplay_assets(&a->gameplay);
    load_win_assets(&a->win);
    load_death_assets(&a->death);
    load_sounds(&a->sounds);
}

void load_screen(State *s) {
    switch (s->screen) {
    case SCREEN_TITLE:
        load_title_screen(&s->screen_state.title);
        break;
    case SCREEN_GAMEPLAY:
        load_gameplay_screen(&s->screen_state.gameplay);
        break;
    case SCREEN_WIN:
        load_win_screen(&s->screen_state.win);
        break;
    case SCREEN_DEATH:
        load_death_screen(&s->screen_state.death);
        break;
    }
}

void load(State *s) {
    s->target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(s->target.texture, TEXTURE_FILTER_POINT);

    InitAudioDevice();
    
    load_assets(&s->assets);
    load_screen(s);

}

void unload_assets(Assets *a) {
    unload_title_assets(&a->title);
    unload_gameplay_assets(&a->gameplay);
    unload_win_assets(&a->win);
    unload_death_assets(&a->death);
    unload_sounds(&a->sounds);
}

void unload(State *s) {
    unload_assets(&s->assets);
    CloseAudioDevice();
    UnloadRenderTexture(s->target);
}

void update(State *s) {
    Screen next_screen = s->screen;

    switch (s->screen) {
    case SCREEN_TITLE:
        update_title(&s->screen_state.title, &next_screen);
        break;
    case SCREEN_GAMEPLAY:
        update_gameplay(&s->screen_state.gameplay, &s->assets.sounds, &next_screen);
        break;
    case SCREEN_WIN:
        update_win(&s->screen_state.win, &next_screen);
        break;
    case SCREEN_DEATH:
        update_death(&s->screen_state.death, &next_screen);
        break;
    }

    if (next_screen != s->screen) {
        s->screen = next_screen;
        memset(&s->screen_state, 0, sizeof(ScreenState));
        load_screen(s);
    }
}

void draw(const State *s) {
    BeginTextureMode(s->target);
        ClearBackground(CLEAR_COLOR);

        switch (s->screen) {
        case SCREEN_TITLE:
            draw_title(&s->screen_state.title, &s->assets.title);
            break;
        case SCREEN_GAMEPLAY:
            draw_gameplay(&s->screen_state.gameplay, &s->assets.gameplay);
            break;
        case SCREEN_WIN:
            draw_win(&s->screen_state.win, &s->assets.win);
            break;
        case SCREEN_DEATH:
            draw_death(&s->screen_state.death, &s->assets.death);
            break;
        }
    EndTextureMode();
    
    BeginDrawing();
        ClearBackground(CLEAR_COLOR);

        int w = GetScreenWidth();
        int h = GetScreenHeight();
        int tw = s->target.texture.width;
        int th = s->target.texture.height;
        int scale = max(min(w / tw, h / th), 1);
        
        Rectangle src = {0, 0, (float)tw, -(float)th};
        Rectangle dest = {(float)(w - tw * scale) / 2, (float)(h - th * scale) / 2, (float)(tw * scale), (float)(th * scale)};
        Vector2 origin = {0, 0};
        DrawTexturePro(s->target.texture, src, dest, origin, 0.0f, WHITE);
    EndDrawing();
}

#if defined(PLATFORM_WEB)
void emscripten_main_loop(void *arg) {
    State *s = arg;
    update(s);
    draw(s);
}
#endif

int main(void) {
    srand(time(NULL));

#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);
#endif

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HEXLOCK");

    State s = {0};
    load(&s);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(emscripten_main_loop, &s, 60, true);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        update(&s);
        draw(&s);
    }
#endif
    
    unload(&s);
    CloseWindow();

    return 0;
}
