#include "glyph.h"

static uint8_t glyph_spawn_rate(uint8_t wave) {
    return GLYPH_SPAWN_RATE - wave * 3 / 4;
}

static void spawn_glyph(SignU8 sign, Glyph glyphs[], uint8_t wave, const Grid grid, const Player *player) {
    bool is_used[18] = {0};

    for (uint8_t y = 0; y < N_ROWS; ++y) {
        for (uint8_t x = 0; x < N_COLS; ++x) {
            Position pos = {x, y};
            if (is_wall(grid, pos) || !grid[y][x].is_player) {
                is_used[y * 6 + x] = true;
            }
        }
    }
    
    is_used[player->pos.y * 6 + player->pos.x] = true;

    switch (player->action) {
    case ACTION_LEFT:
        is_used[player->pos.y * 6 + (player->pos.x - 1)] = true;
        break;
    case ACTION_RIGHT:
        is_used[player->pos.y * 6 + (player->pos.x + 1)] = true;
        break;
    case ACTION_UP:
        is_used[(player->pos.y - 1) * 6 + player->pos.x] = true;
        break;
    case ACTION_DOWN:
        is_used[(player->pos.y + 1) * 6 + player->pos.x] = true;
        break;
    }

    for (size_t i = 0; i < MAX_GLYPHS; ++i) {
        Glyph *glyph = &glyphs[i];
        if (glyph->exists) {
            is_used[glyph->pos.y * 6 + glyph->pos.x] = true;
        }
    }

    bool has_available = false;
    for (size_t i = 0; i < 18; ++i) {
        if (!is_used[i]) {
            has_available = true;
            break;
        }
    }
    if (!has_available) return;

    Position pos;
    do {
        pos.x = rand() % 6;
        pos.y = rand() % 3;
    } while (is_used[pos.y * 6 + pos.x]);

    for (size_t i = 0; i < MAX_GLYPHS; ++i) {
        Glyph *glyph = &glyphs[i];
        if (!glyph->exists) {
            glyph->exists = true;
            glyph->pos = pos;
            glyph->t = 0.0f;
            glyph->sign = sign;
            glyph->lifetime = glyph_spawn_rate(wave) * MAX_GLYPHS;
            break;
        }
    }
}

void update_glyphs(Glyph glyphs[]) {
    for (size_t i = 0; i < MAX_GLYPHS; ++i) {
        Glyph *glyph = &glyphs[i];
        if (glyph->exists) {
            glyph->lifetime--;
            if (glyph->lifetime == 0) glyph->exists = false;
        }
    }
}

void update_glyph_spawner(GlyphSpawner *spawner, Glyph glyphs[], uint8_t wave, const Grid grid, const Player *player) {
    spawner->cooldown--;
    if (spawner->cooldown == 0) {
        SignU8 sign = spawner->chunk[spawner->index++];
        spawn_glyph(sign, glyphs, wave, grid, player);
        spawner->cooldown = glyph_spawn_rate(wave);

        if (spawner->index == 6) {
            generate_glyph_chunk(spawner);
            spawner->index = 0;
        }
    }
}

void update_glyphs_time(Glyph glyphs[], float dt) {
    for (size_t i = 0; i < MAX_GLYPHS; ++i) {
        Glyph *glyph = &glyphs[i];
        if (glyph->exists) {
            if (glyph->lifetime == 1) {
                glyph->t = max(glyph->t - dt / TICK_RATE, 0.0f);
            } else if (glyph->t < 1.0f) {
                glyph->t = min(glyph->t + dt / TICK_RATE, 1.0f);
            }
        }
    }
}
