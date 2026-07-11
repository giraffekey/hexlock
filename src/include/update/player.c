#include "player.h"

void handle_input(Player *player, const Grid grid) {
    if (player->cooldown > 0 && player->next_action != ACTION_WAIT) return;

    bool is_gamepad_left_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || is_gamepad_left_pressed) {
        if (player->cooldown > 0) {
            Position next_pos = {player->pos.x - 1 - (player->action == ACTION_LEFT), player->pos.y};
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                player->next_action = ACTION_LEFT;
                player->next_cooldown = player->statuses[STATUS_MOVE_FAST] > 1 ? 1 : 2;
                player->next_t_rate = (float)player->next_cooldown;
                return;
            }
        } else {
            Position next_pos = {player->pos.x - 1, player->pos.y};
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                player->action = ACTION_LEFT;
                player->cooldown = player->statuses[STATUS_MOVE_FAST] ? 1 : 2;
                player->t_rate = (float)player->cooldown;
                return;
            }
        }
    }

    bool is_gamepad_right_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) || is_gamepad_right_pressed) {
        if (player->cooldown > 0) {
            Position next_pos = {player->pos.x + 1 + (player->action == ACTION_RIGHT), player->pos.y};
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                player->next_action = ACTION_RIGHT;
                player->next_cooldown = player->statuses[STATUS_MOVE_FAST] > 1 ? 1 : 2;
                player->next_t_rate = (float)player->next_cooldown;
                return;
            }
        } else {
            Position next_pos = {player->pos.x + 1, player->pos.y};
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                player->action = ACTION_RIGHT;
                player->cooldown = player->statuses[STATUS_MOVE_FAST] ? 1 : 2;
                player->t_rate = (float)player->cooldown;
                return;
            }
        }
    }

    bool is_gamepad_up_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || is_gamepad_up_pressed) {
        if (player->cooldown > 0) {
            Position next_pos = {player->pos.x, player->pos.y - 1 - (player->action == ACTION_UP)};
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                player->next_action = ACTION_UP;
                player->next_cooldown = player->statuses[STATUS_MOVE_FAST] > 1 ? 1 : 2;
                player->next_t_rate = (float)player->next_cooldown;
                return;
            }
        } else {
            Position next_pos = {player->pos.x, player->pos.y - 1};
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                player->action = ACTION_UP;
                player->cooldown = player->statuses[STATUS_MOVE_FAST] ? 1 : 2;
                player->t_rate = (float)player->cooldown;
                return;
            }
        }
    }

    bool is_gamepad_down_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || is_gamepad_down_pressed) {
        if (player->cooldown > 0) {
            Position next_pos = {player->pos.x, player->pos.y + 1 + (player->action == ACTION_DOWN)};
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                player->next_action = ACTION_DOWN;
                player->next_cooldown = player->statuses[STATUS_MOVE_FAST] > 1 ? 1 : 2;
                player->next_t_rate = (float)player->next_cooldown;
                return;
            }
        } else {
            Position next_pos = {player->pos.x, player->pos.y + 1};
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                player->action = ACTION_DOWN;
                player->cooldown = player->statuses[STATUS_MOVE_FAST] ? 1 : 2;
                player->t_rate = (float)player->cooldown;
                return;
            }
        }
    }

    bool is_gamepad_missile_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_SPACE) || is_gamepad_missile_pressed) {
        if (player->missile_delay == 0) {
            if (player->cooldown > 0) {
                player->next_action = ACTION_MISSILE;
                player->next_cooldown = player->statuses[STATUS_CAST_FAST] > 1 ? 1 : 2;
                player->next_t_rate = (float)player->next_cooldown;
                return;
            } else {
                player->action = ACTION_MISSILE;
                player->cooldown = player->statuses[STATUS_CAST_FAST] ? 1 : 2;
                player->t_rate = (float)player->cooldown;
                player->missile_delay = player->statuses[STATUS_CAST_FAST] ? 2 : 4;
                return;
            }
        }
    }

    bool is_gamepad_hex_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP);
    if (IsKeyPressed(KEY_X) || IsKeyPressed(KEY_F) || is_gamepad_hex_pressed) {
        if (player->hexes[0].valid) {
            if (player->cooldown > 0) {
                if (player->action != ACTION_HEX) {
                    player->next_action = ACTION_HEX;
                    player->next_cooldown = player->statuses[STATUS_CAST_FAST] > 1 ? 1 : 2;
                    player->next_t_rate = (float)player->next_cooldown;
                    return;
                }
            } else {
                player->action = ACTION_HEX;
                player->cooldown = player->statuses[STATUS_CAST_FAST] ? 1 : 2;
                player->t_rate = (float)player->cooldown;
                return;
            }
        }
    }
}

static void cast_aries(const Player *player, Bullet bullets[]) {
    spawn_bullet(bullets, player->pos, BULLET_FIRE_WAVE, true);
}

static void cast_aries_omega(const Player *player, Bullet bullets[]) {
    spawn_bullet(bullets, player->pos, BULLET_FIRE_BLAST, true);
}

static void cast_taurus(const Player *player, Grid grid) {
    Position pos = {player->pos.x + 1, player->pos.y};
    set_tile(grid, pos, TILE_ROCK, 10);
}

static void cast_taurus_omega(const Player *player, Grid grid) {
    Position pos = {player->pos.x + 1, player->pos.y};
    set_tile(grid, pos, TILE_CRYSTAL, 15);
}

static void cast_gemini(Player *player) {
    player->statuses[STATUS_MOVE_FAST] += 50;
}

static void cast_gemini_omega(Player *player) {
    player->statuses[STATUS_MOVE_FAST] += 100;
    player->statuses[STATUS_CAST_FAST] += 100;
}

static void cast_cancer(const Player *player, Bullet bullets[]) {
    BulletData data;
    data.whirlpool.is_moving = true;
    data.whirlpool.lifetime = 3;
    spawn_bullet_with_data(bullets, player->pos, BULLET_WHIRLPOOL, true, data);
}

static void cast_cancer_omega(const Player *player, Bullet bullets[]) {
    BulletData data;
    data.whirlpool.is_moving = true;
    data.whirlpool.lifetime = 5;
    spawn_bullet_with_data(bullets, player->pos, BULLET_HURRICANE, true, data);
}

static void cast_leo(Player *player) {
    player->statuses[STATUS_FIRE_CLOAK] += 50;
}

static void cast_leo_omega(Player *player) {
    player->statuses[STATUS_WILDFIRE_CLOAK] += 100;
}

static void cast_virgo(const Player *player, Grid grid) {
    for (uint8_t x = player->pos.x; x < N_COLS; ++x) {
        Tile *tile = &grid[player->pos.y][x];
        if (!tile->is_player) {
            tile->is_player = true;
            break;
        }
    }
}

static void cast_virgo_omega(const Player *player, Grid grid) {
    for (uint8_t x = player->pos.x; x < N_COLS; ++x) {
        bool is_player_col = true;
        for (uint8_t y = 0; y < N_ROWS; ++y) {
            Tile *tile = &grid[y][x];
            if (!tile->is_player) {
                is_player_col = false;
                break;
            }
        }

        if (!is_player_col) {
            for (uint8_t y = 0; y < N_ROWS; ++y) {
                grid[y][x].is_player = true;
            }
            break;
        }
    }
}

static void cast_libra(const Player *player, Bullet bullets[]) {
    Position pos = {player->pos.x, 1};
    BulletData data;
    data.twister.cooldown = 0;
    data.twister.is_moving_up = rand() % 2;
    spawn_bullet_with_data(bullets, pos, BULLET_TWISTER, true, data);
}

static void cast_libra_omega(const Player *player, Bullet bullets[]) {
    Position pos = {player->pos.x, 1};
    BulletData data;
    data.twister.cooldown = 0;
    data.twister.is_moving_up = true;
    spawn_bullet_with_data(bullets, pos, BULLET_TWISTER, true, data);
    data.twister.is_moving_up = false;
    spawn_bullet_with_data(bullets, pos, BULLET_TWISTER, true, data);
}

static void cast_scorpio(Player *player) {
    player->statuses[STATUS_LEECH] += 1;
}

static void cast_scorpio_omega(Player *player) {
    player->statuses[STATUS_LEECH] += 1;
    player->statuses[STATUS_REFLECT] += 1;
}

static void cast_sagittarius(const Player *player, Grid grid) {
    Position pos = {player->pos.x + 3, player->pos.y};
    set_tile(grid, pos, TILE_MAGMA, 0);
}

static void cast_sagittarius_omega(const Player *player, Grid grid) {
    for (uint8_t x = N_COLS / 2; x < N_COLS; ++x) {
        Position pos = {x, player->pos.y};
        set_tile(grid, pos, TILE_MAGMA, 0);
    }
}

static void cast_capricorn(const Player *player, Bullet bullets[]) {
    spawn_bullet(bullets, player->pos, BULLET_SHOCKWAVE, true);
}

static void cast_capricorn_omega(const Player *player, Bullet bullets[]) {
    spawn_bullet(bullets, (Position){player->pos.x, 0}, BULLET_SHOCKWAVE, true);
    spawn_bullet(bullets, (Position){player->pos.x, 1}, BULLET_SHOCKWAVE, true);
    spawn_bullet(bullets, (Position){player->pos.x, 2}, BULLET_SHOCKWAVE, true);
}

static void cast_aquarius(Player *player) {
    player->statuses[STATUS_PHANTOM] += 30;
}

static void cast_aquarius_omega(Player *player) {
    player->statuses[STATUS_PHANTOM] += 50;
}

static void cast_pisces(Player *player) {
    player->hp = min(player->hp + 1, MAX_PLAYER_HEALTH);
}

static void cast_pisces_omega(Player *player) {
    player->hp = MAX_PLAYER_HEALTH;
}

static void cast_hex(Player *player, Grid grid, Bullet bullets[]) {
    Hex hex = player->hexes[0];
    if (hex.valid) {
        switch (hex.sign + hex.omega * N_SIGNS) {
        case ARIES:
            cast_aries(player, bullets);
            break;
        case ARIES + N_SIGNS:
            cast_aries_omega(player, bullets);
            break;
        case TAURUS:
            cast_taurus(player, grid);
            break;
        case TAURUS + N_SIGNS:
            cast_taurus_omega(player, grid);
            break;
        case GEMINI:
            cast_gemini(player);
            break;
        case GEMINI + N_SIGNS:
            cast_gemini_omega(player);
            break;
        case CANCER:
            cast_cancer(player, bullets);
            break;
        case CANCER + N_SIGNS:
            cast_cancer_omega(player, bullets);
            break;
        case LEO:
            cast_leo(player);
            break;
        case LEO + N_SIGNS:
            cast_leo_omega(player);
            break;
        case VIRGO:
            cast_virgo(player, grid);
            break;
        case VIRGO + N_SIGNS:
            cast_virgo_omega(player, grid);
            break;
        case LIBRA:
            cast_libra(player, bullets);
            break;
        case LIBRA + N_SIGNS:
            cast_libra_omega(player, bullets);
            break;
        case SCORPIO:
            cast_scorpio(player);
            break;
        case SCORPIO + N_SIGNS:
            cast_scorpio_omega(player);
            break;
        case SAGITTARIUS:
            cast_sagittarius(player, grid);
            break;
        case SAGITTARIUS + N_SIGNS:
            cast_sagittarius_omega(player, grid);
            break;
        case CAPRICORN:
            cast_capricorn(player, bullets);
            break;
        case CAPRICORN + N_SIGNS:
            cast_capricorn_omega(player, bullets);
            break;
        case AQUARIUS:
            cast_aquarius(player);
            break;
        case AQUARIUS + N_SIGNS:
            cast_aquarius_omega(player);
            break;
        case PISCES:
            cast_pisces(player);
            break;
        case PISCES + N_SIGNS:
            cast_pisces_omega(player);
            break;
        }
    }
    use_hex(player);
}

void update_player(Player *player, Grid grid, Glyph glyphs[], Enemy enemies[], Bullet bullets[]) {
    if (player->cooldown > 0) player->cooldown--;

    if (player->missile_delay > 0) player->missile_delay--;

    if (player->cooldown == 0) {
        switch (player->action) {
        case ACTION_WAIT:
            break;
        case ACTION_LEFT:
            player->pos.x--;
            check_player_glyph_collision(player, glyphs);
            check_player_enemy_collision(player, enemies);
            check_player_bullet_collision(player, bullets);
            break;
        case ACTION_RIGHT:
            player->pos.x++;
            check_player_glyph_collision(player, glyphs);
            check_player_enemy_collision(player, enemies);
            check_player_bullet_collision(player, bullets);
            break;
        case ACTION_UP:
            player->pos.y--;
            check_player_glyph_collision(player, glyphs);
            check_player_enemy_collision(player, enemies);
            check_player_bullet_collision(player, bullets);
            break;
        case ACTION_DOWN:
            player->pos.y++;
            check_player_glyph_collision(player, glyphs);
            check_player_enemy_collision(player, enemies);
            check_player_bullet_collision(player, bullets);
            break;
        case ACTION_MISSILE:
            spawn_bullet(bullets, player->pos, BULLET_MISSILE, true);
            break;
        case ACTION_HEX:
            cast_hex(player, grid, bullets);
            break;
        }

        for (Status status = 0; status < N_STATUSES; ++status) {
            if (player->statuses[status] && status != STATUS_LEECH && status != STATUS_REFLECT) {
                player->statuses[status]--;
            }
        }

        if (get_tile_type(grid, player->pos) == TILE_MAGMA) {
            damage_player(player, 1);
        }

        player->action = player->next_action;
        player->cooldown = player->next_cooldown;
        player->t_rate = player->next_t_rate;
        player->next_action = ACTION_WAIT;
        player->next_cooldown = 0;
        player->next_t_rate = 2.0f;
        player->t = 0.0f;

        player->wait_frame = player->action == ACTION_WAIT ? (player->wait_frame + 1) % 4 : 1;

        if (player->action == ACTION_MISSILE) {
            player->missile_delay = player->statuses[STATUS_CAST_FAST] ? 2 : 4;
        }
    }
}

void update_target_enemy(size_t *target_enemy, const Player *player, const Enemy enemies[]) {
    uint8_t min_x = N_COLS;
    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        const Enemy *enemy = &enemies[i];
        if (enemy->exists && enemy->pos.x > player->pos.x && enemy->pos.y == player->pos.y && enemy->pos.x < min_x) {
            *target_enemy = i;
            min_x = enemy->pos.x;
        }
    }
}

void update_player_time(Player *player, float dt) {
    player->t = min(player->t + dt / TICK_RATE / player->t_rate, 1.0f);
}
