#include "player.h"

/**
 * Get the next position of the player based on their action
 * 
 * @param pos The current position
 * @param action The current action
 */
static Position get_next_position(Position pos, ActionU8 action) {
    switch (action) {
    case ACTION_LEFT: return (Position){pos.x - 1, pos.y};
    case ACTION_RIGHT: return (Position){pos.x + 1, pos.y};
    case ACTION_UP: return (Position){pos.x, pos.y - 1};
    case ACTION_DOWN: return (Position){pos.x, pos.y + 1};
    default: return pos;
    }
    return pos;
}

/**
 * Sets player's current action
 * 
 * @param player The current state of the player
 * @param action The action to be set
 * @param cooldown The amount of ticks the action will take
 */
static void set_player_action(Player *player, ActionU8 action, uint8_t cooldown) {
    player->action = action;
    player->cooldown = cooldown;
    player->t_rate = (float)cooldown;
}

/**
 * Sets player's next action
 * 
 * @param player The current state of the player
 * @param action The action to be set
 * @param cooldown The amount of ticks the action will take
 */
static void set_next_player_action(Player *player, ActionU8 action, uint8_t cooldown) {
    player->next_action = action;
    player->next_cooldown = cooldown;
    player->next_t_rate = (float)cooldown;
}

/**
 * Check if position is in player side bounds
 * 
 * @param pos The position
 * @param offset The offset of the screen
 * @param scale The scale of the screen
 */
static bool is_pos_on_player_side(Vector2 pos, Vector2 offset, float scale) {
    return pos.x >= offset.x + 22 * scale && pos.x <= offset.x + 86 * scale && pos.y >= offset.y + 80 * scale && pos.y <= offset.y + 144 * scale;
}

/**
 * Check if position is in enemy side bounds
 * 
 * @param pos The position
 * @param offset The offset of the screen
 * @param scale The scale of the screen
 */
static bool is_pos_on_enemy_side(Vector2 pos, Vector2 offset, float scale) {
    return pos.x >= offset.x + 94 * scale && pos.x <= offset.x + 158 * scale && pos.y >= offset.y + 80 * scale && pos.y <= offset.y + 144 * scale;
}

/**
 * Check if position is at the top of the screen
 * 
 * @param pos The position
 * @param offset The offset of the screen
 * @param scale The scale of the screen
 */
static bool is_pos_at_top(Vector2 pos, Vector2 offset, float scale) {
    return pos.x >= offset.x && pos.x <= offset.x + GAME_WIDTH * scale && pos.y >= offset.y && pos.y < offset.y + 80 * scale;
}

/**
 * Check if position is at a selectable hex
 * 
 * @param pos The position
 * @param offset The offset of the screen
 * @param scale The scale of the screen
 */
static bool is_pos_at_hex(Vector2 pos, size_t i, Vector2 offset, float scale) {
    float left_x = (float)(10 + i * 16);
    float right_x = (float)(26 + i * 16);
    return pos.x >= offset.x + left_x * scale && pos.x <= offset.x + right_x * scale && pos.y >= offset.y + 150 * scale && pos.y <= offset.y + 166 * scale;
}

/**
 * Updates player action in response to keyboard, gamepad, mouse, and touch input
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 */
void handle_input(Player *player, const Grid grid) {
    float w = (float)GetScreenWidth();
    float h = (float)GetScreenHeight();
    float scale = max(min(w / GAME_WIDTH, h / GAME_HEIGHT), 1.0f);
    Vector2 offset = {(w - GAME_WIDTH * scale) / 2.0f, (h - GAME_HEIGHT * scale) / 2.0f};

    Gesture gesture = GetGestureDetected();
    if (gesture == GESTURE_TAP || gesture == GESTURE_DOUBLETAP) player->tap = GetTouchPosition(0);

    bool is_key_pressed, is_gamepad_pressed, is_click, is_scroll, is_gesture;

    size_t n_hexes = min(player->n_hexes, MAX_HEXES);

    /* Handle input for previous hex selection */

    is_key_pressed = IsKeyPressed(KEY_TAB) && IsKeyDown(KEY_LEFT_SHIFT);
    is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
    is_scroll = GetMouseWheelMove() > 0.0f;

    if (is_key_pressed || is_gamepad_pressed || is_scroll) {
        if (n_hexes > 0) {
            player->selected = (player->selected + n_hexes - 1) % n_hexes;
        }
    }

    /* Handle input for next hex selection */

    is_key_pressed = IsKeyPressed(KEY_C) || IsKeyPressed(KEY_TAB) && !IsKeyDown(KEY_LEFT_SHIFT);
    is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
    is_scroll = GetMouseWheelMove() < 0.0f;

    if (is_key_pressed || is_gamepad_pressed || is_scroll) {
        if (n_hexes > 0) {
            player->selected = (player->selected + 1) % n_hexes;
        }
    }

    /* Handle input for clicking on hex */

    for (size_t i = 0; i < n_hexes; ++i) {
        is_click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && is_pos_at_hex(GetMousePosition(), i, offset, scale);
        is_gesture = gesture == GESTURE_TAP && is_pos_at_hex(player->tap, i, offset, scale);
        if (is_click || is_gesture) {
            player->selected = i;
            break;
        }
    }

    // Only up to two actions can be queued
    if (player->cooldown > 0 && player->next_action != ACTION_WAIT) return;

    /* Handle input for move left action */

    is_key_pressed = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
    is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    is_gesture = gesture == GESTURE_SWIPE_LEFT && is_pos_on_player_side(player->tap, offset, scale);

    if (is_key_pressed || is_gamepad_pressed || is_gesture) {
        if (player->cooldown > 0) {
            Position to = get_next_position(player->pos, player->action);
            Position next_pos = get_next_position(to, ACTION_LEFT);
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                set_next_player_action(player, ACTION_LEFT, player->statuses[STATUS_MOVE_FAST] > 1 ? 1 : 2);
                return;
            }
        } else {
            Position next_pos = get_next_position(player->pos, ACTION_LEFT);
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                set_player_action(player, ACTION_LEFT, player->statuses[STATUS_MOVE_FAST] ? 1 : 2);
                return;
            }
        }
    }

    /* Handle input for move right action */

    is_key_pressed = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);
    is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    is_gesture = gesture == GESTURE_SWIPE_RIGHT && is_pos_on_player_side(player->tap, offset, scale);

    if (is_key_pressed || is_gamepad_pressed || is_gesture) {
        if (player->cooldown > 0) {
            Position to = get_next_position(player->pos, player->action);
            Position next_pos = get_next_position(to, ACTION_RIGHT);
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                set_next_player_action(player, ACTION_RIGHT, player->statuses[STATUS_MOVE_FAST] > 1 ? 1 : 2);
                return;
            }
        } else {
            Position next_pos = get_next_position(player->pos, ACTION_RIGHT);
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                set_player_action(player, ACTION_RIGHT, player->statuses[STATUS_MOVE_FAST] ? 1 : 2);
                return;
            }
        }
    }

    /* Handle input for move up action */

    is_key_pressed = IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
    is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP);
    is_gesture = gesture == GESTURE_SWIPE_UP && is_pos_on_player_side(player->tap, offset, scale);

    if (is_key_pressed || is_gamepad_pressed || is_gesture) {
        if (player->cooldown > 0) {
            Position to = get_next_position(player->pos, player->action);
            Position next_pos = get_next_position(to, ACTION_UP);
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                set_next_player_action(player, ACTION_UP, player->statuses[STATUS_MOVE_FAST] > 1 ? 1 : 2);
                return;
            }
        } else {
            Position next_pos = get_next_position(player->pos, ACTION_UP);
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                set_player_action(player, ACTION_UP, player->statuses[STATUS_MOVE_FAST] ? 1 : 2);
                return;
            }
        }
    }

    /* Handle input for move down action */

    is_key_pressed = IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
    is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    is_gesture = gesture == GESTURE_SWIPE_DOWN && is_pos_on_player_side(player->tap, offset, scale);

    if (is_key_pressed || is_gamepad_pressed || is_gesture) {
        if (player->cooldown > 0) {
            Position to = get_next_position(player->pos, player->action);
            Position next_pos = get_next_position(to, ACTION_DOWN);
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                set_next_player_action(player, ACTION_DOWN, player->statuses[STATUS_MOVE_FAST] > 1 ? 1 : 2);
                return;
            }
        } else {
            Position next_pos = get_next_position(player->pos, ACTION_DOWN);
            if (is_in_player_bounds(grid, next_pos) && !is_wall(grid, next_pos)) {
                set_player_action(player, ACTION_DOWN, player->statuses[STATUS_MOVE_FAST] ? 1 : 2);
                return;
            }
        }
    }

    /* Handle input for cast missile action */

    is_key_pressed = IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_SPACE);
    is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
    is_click = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && is_pos_on_enemy_side(GetMousePosition(), offset, scale);
    is_gesture = gesture == GESTURE_TAP && !IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && is_pos_on_enemy_side(player->tap, offset, scale);

    if (is_key_pressed || is_gamepad_pressed || is_click || is_gesture) {
        if (player->missile_delay == 0) {
            if (player->cooldown > 0) {
                set_next_player_action(player, ACTION_MISSILE, player->statuses[STATUS_CAST_FAST] > 1 ? 1 : 2);
                return;
            } else {
                set_player_action(player, ACTION_MISSILE, player->statuses[STATUS_CAST_FAST] ? 1 : 2);
                player->missile_delay = player->statuses[STATUS_CAST_FAST] ? 2 : 4;
                return;
            }
        }
    }

    /* Handle input for cast hex action */

    is_key_pressed = IsKeyPressed(KEY_X) || IsKeyPressed(KEY_F);
    is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP);
    is_click = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && is_pos_on_enemy_side(GetMousePosition(), offset, scale);
    is_gesture = gesture == GESTURE_DOUBLETAP && is_pos_at_top(player->tap, offset, scale);

    if (is_key_pressed || is_gamepad_pressed || is_click || is_gesture) {
        if (player->hexes[0].valid) {
            if (player->cooldown > 0) {
                if (player->action != ACTION_HEX) {
                    set_next_player_action(player, ACTION_HEX, player->statuses[STATUS_CAST_FAST] > 1 ? 1 : 2);
                    return;
                }
            } else {
                set_player_action(player, ACTION_HEX, player->statuses[STATUS_CAST_FAST] ? 1 : 2);
                return;
            }
        }
    }
}

/**
 * Move the player based on their current action
 * 
 * @param player The current state of the player
 * @param glyphs The current state of the arena's glyphs
 * @param enemies The current state of the arena's enemies
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void move_player(Player *player, Glyph glyphs[], Enemy enemies[], Bullet bullets[], uint16_t *score, const Sounds *sounds) {
    switch (player->action) {
    case ACTION_LEFT:
        player->pos.x--;
        break;
    case ACTION_RIGHT:
        player->pos.x++;
        break;
    case ACTION_UP:
        player->pos.y--;
        break;
    case ACTION_DOWN:
        player->pos.y++;
        break;
    }

    // Check for collisions after movement
    check_player_glyph_collision(player, glyphs, score, sounds);
    check_player_enemy_collision(player, enemies, bullets, sounds);
    check_player_bullet_collision(player, bullets, sounds);
}

/**
 * Push the enemy back if their current space is occupied
 * 
 * @param from The position being checked
 * @param grid The current state of the arena's tiles
 * @param enemies The current state of the arena's enemies
 */
static void push_enemy(Position from, const Grid grid, Enemy enemies[]) {
    bool is_used[18] = {0};

    for (uint8_t y = 0; y < N_ROWS; ++y) {
        for (uint8_t x = 0; x < N_COLS; ++x) {
            Position pos = {x, y};
            if (is_wall(grid, pos) || is_player_tile(grid, pos)) is_used[y * 6 + x] = true;
        }
    }

    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        const Enemy *enemy = &enemies[i];
        if (enemy->exists) is_used[enemy->pos.y * 6 + enemy->pos.x] = true;
    }

    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        Enemy *enemy = &enemies[i];
        if (enemy->exists && is_pos_eq(enemy->pos, from)) {
            if (enemy->pos.x == N_COLS - 1) {
                // Kill the enemy if they are in the last column
                enemy->exists = false;
                // TODO: Play death animation
            } else {
                Position behind = {enemy->pos.x + 1, enemy->pos.y};
                if (is_used[behind.y * 6 + behind.x]) {
                    // Kill the enemy if no space behind them exists
                    enemy->exists = false;
                    // TODO: Play death animation
                } else {
                    // Otherwise, move them to the space behind
                    enemy->pos = behind;
                    if (enemy->type == FLUFFY) enemy->data.fluffy.origin = behind;
                    // TODO: Play push animation
                }
            }
        }
    }
}

/**
 * Casts Aries hex - Spawn fire wave that hits neighbors in a V shape
 * 
 * @param player The current state of the player
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_aries(const Player *player, Bullet bullets[], const Sounds *sounds) {
    spawn_bullet(bullets, player->pos, BULLET_FIRE_WAVE, true);
    PlaySound(sounds->special);
}

/**
 * Casts AriesΩ hex - Spawn fire blast that hits neighbors in an X shape
 * 
 * @param player The current state of the player
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_aries_omega(const Player *player, Bullet bullets[], const Sounds *sounds) {
    spawn_bullet(bullets, player->pos, BULLET_FIRE_BLAST, true);
    PlaySound(sounds->special);
}

/**
 * Casts Taurus hex - Place a rock in front of player that reflects bullets
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param enemies The current state of the arena's enemies
 * @param sounds The available sound effects to be played
 */
static void cast_taurus(const Player *player, Grid grid, Enemy enemies[], const Sounds *sounds) {
    Position pos = {player->pos.x + 1, player->pos.y};
    set_tile(grid, pos, TILE_ROCK, 10);
    push_enemy(pos, grid, enemies);
    PlaySound(sounds->place);
}

/**
 * Casts TaurusΩ hex - Place a crystal in front of player that reflects bullets
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param enemies The current state of the arena's enemies
 * @param sounds The available sound effects to be played
 */
static void cast_taurus_omega(const Player *player, Grid grid, Enemy enemies[], const Sounds *sounds) {
    Position pos = {player->pos.x + 1, player->pos.y};
    set_tile(grid, pos, TILE_CRYSTAL, 15);
    push_enemy(pos, grid, enemies);
    PlaySound(sounds->place);
}

/**
 * Casts Gemini hex - Move faster for 50 ticks
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_gemini(Player *player, const Sounds *sounds) {
    player->statuses[STATUS_MOVE_FAST] += 50;
    PlaySound(sounds->power);
}

/**
 * Casts GeminiΩ hex - Move and cast faster for 100 ticks
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_gemini_omega(Player *player, const Sounds *sounds) {
    player->statuses[STATUS_MOVE_FAST] += 100;
    player->statuses[STATUS_CAST_FAST] += 100;
    PlaySound(sounds->power);
}

/**
 * Casts Cancer hex - Spawn whirlpool for 3 ticks that damages four neighbors
 * 
 * @param player The current state of the player
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_cancer(const Player *player, Bullet bullets[], const Sounds *sounds) {
    BulletData data;
    data.whirlpool.is_moving = true;
    data.whirlpool.lifetime = 3;
    data.whirlpool.is_child = false;
    spawn_bullet_with_data(bullets, player->pos, BULLET_WHIRLPOOL, true, data);
    PlaySound(sounds->special);
}

/**
 * Casts CancerΩ hex - Spawn hurricane for 5 ticks that damages eight neighbors
 * 
 * @param player The current state of the player
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_cancer_omega(const Player *player, Bullet bullets[], const Sounds *sounds) {
    BulletData data;
    data.whirlpool.is_moving = true;
    data.whirlpool.lifetime = 5;
    data.whirlpool.is_child = false;
    spawn_bullet_with_data(bullets, player->pos, BULLET_HURRICANE, true, data);
    PlaySound(sounds->special);
}

/**
 * Casts Leo hex - For the next 50 ticks, spawn fireball when player is hit
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_leo(Player *player, const Sounds *sounds) {
    player->statuses[STATUS_FIRE_CLOAK] += 50;
    PlaySound(sounds->power);
}

/**
 * Casts LeoΩ hex - For the next 100 ticks, spawn wildfire when player is hit
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_leo_omega(Player *player, const Sounds *sounds) {
    player->statuses[STATUS_WILDFIRE_CLOAK] += 100;
    PlaySound(sounds->power);
}

/**
 * Casts Virgo hex - Claim the closest tile as a player tile
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param enemies The current state of the arena's enemies
 * @param sounds The available sound effects to be played
 */
static void cast_virgo(const Player *player, Grid grid, Enemy enemies[], const Sounds *sounds) {
    for (uint8_t x = player->pos.x; x < N_COLS; ++x) {
        Tile *tile = &grid[player->pos.y][x];
        if (!tile->is_player) {
            tile->is_player = true;
            push_enemy((Position){x, player->pos.y}, grid, enemies);
            break;
        }
    }
    PlaySound(sounds->advance);
}

/**
 * Casts VirgoΩ hex - Claim the next column as player tiles
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param enemies The current state of the arena's enemies
 * @param sounds The available sound effects to be played
 */
static void cast_virgo_omega(const Player *player, Grid grid, Enemy enemies[], const Sounds *sounds) {
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
                push_enemy((Position){x, y}, grid, enemies);
            }
            break;
        }
    }
    PlaySound(sounds->advance);
}

/**
 * Casts Libra hex - Spawn one diagonally moving twister
 * 
 * @param player The current state of the player
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_libra(const Player *player, Bullet bullets[], const Sounds *sounds) {
    Position pos = {player->pos.x, 1};
    BulletData data;
    data.twister.cooldown = 0;
    data.twister.is_moving_up = rand() % 2;
    spawn_bullet_with_data(bullets, pos, BULLET_TWISTER, true, data);
    PlaySound(sounds->special);
}

/**
 * Casts LibraΩ hex - Spawn two diagonally moving twisters
 * 
 * @param player The current state of the player
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_libra_omega(const Player *player, Bullet bullets[], const Sounds *sounds) {
    Position pos = {player->pos.x, 1};
    BulletData data;
    data.twister.cooldown = 0;
    data.twister.is_moving_up = true;
    spawn_bullet_with_data(bullets, pos, BULLET_TWISTER, true, data);
    data.twister.is_moving_up = false;
    spawn_bullet_with_data(bullets, pos, BULLET_TWISTER, true, data);
    PlaySound(sounds->special);
}

/**
 * Casts Scorpio hex - Heal from next attack
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_scorpio(Player *player, const Sounds *sounds) {
    player->statuses[STATUS_LEECH] += 1;
    PlaySound(sounds->power);
}

/**
 * Casts ScorpioΩ hex - Heal from and reflect next attack
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_scorpio_omega(Player *player, const Sounds *sounds) {
    player->statuses[STATUS_LEECH] += 1;
    player->statuses[STATUS_REFLECT] += 1;
    PlaySound(sounds->power);
}

/**
 * Casts Sagittarius hex - Place magma 3 tiles ahead
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param sounds The available sound effects to be played
 */
static void cast_sagittarius(const Player *player, Grid grid, const Sounds *sounds) {
    Position pos = {player->pos.x + 3, player->pos.y};
    set_tile(grid, pos, TILE_MAGMA, 0);
    PlaySound(sounds->place);
}

/**
 * Casts SagittariusΩ hex - Fill enemy row with magma
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param sounds The available sound effects to be played
 */
static void cast_sagittarius_omega(const Player *player, Grid grid, const Sounds *sounds) {
    for (uint8_t x = N_COLS / 2; x < N_COLS; ++x) {
        Position pos = {x, player->pos.y};
        set_tile(grid, pos, TILE_MAGMA, 0);
    }
    PlaySound(sounds->place);
}

/**
 * Casts Capricorn hex - Spawn shockwave in one row
 * 
 * @param player The current state of the player
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_capricorn(const Player *player, Bullet bullets[], const Sounds *sounds) {
    spawn_bullet(bullets, player->pos, BULLET_SHOCKWAVE, true);
    PlaySound(sounds->special);
}

/**
 * Casts CapricornΩ hex - Spawn shockwave in all rows
 * 
 * @param player The current state of the player
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_capricorn_omega(const Player *player, Bullet bullets[], const Sounds *sounds) {
    spawn_bullet(bullets, (Position){player->pos.x, 0}, BULLET_SHOCKWAVE, true);
    spawn_bullet(bullets, (Position){player->pos.x, 1}, BULLET_SHOCKWAVE, true);
    spawn_bullet(bullets, (Position){player->pos.x, 2}, BULLET_SHOCKWAVE, true);
    PlaySound(sounds->special);
}

/**
 * Casts Aquarius hex - Become phantom for 30 ticks
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_aquarius(Player *player, const Sounds *sounds) {
    player->statuses[STATUS_PHANTOM] += 30;
    PlaySound(sounds->power);
}

/**
 * Casts AquariusΩ hex - Become phantom for 50 ticks
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_aquarius_omega(Player *player, const Sounds *sounds) {
    player->statuses[STATUS_PHANTOM] += 50;
    PlaySound(sounds->power);
}

/**
 * Casts Pisces hex - Heal player 1 HP
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_pisces(Player *player, const Sounds *sounds) {
    player->hp = min(player->hp + 1, MAX_PLAYER_HEALTH);
    PlaySound(sounds->heal);
}

/**
 * Casts PiscesΩ hex - Heal player to full HP
 * 
 * @param player The current state of the player
 * @param sounds The available sound effects to be played
 */
static void cast_pisces_omega(Player *player, const Sounds *sounds) {
    player->hp = MAX_PLAYER_HEALTH;
    PlaySound(sounds->heal);
}

/**
 * Casts the currently selected hex
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param enemies The current state of the arena's enemies
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
static void cast_hex(Player *player, Grid grid, Enemy enemies[], Bullet bullets[], const Sounds *sounds) {
    Hex hex = player->hexes[player->selected];
    if (hex.valid) {
        switch (hex.sign + hex.omega * N_SIGNS) {
        case ARIES:
            cast_aries(player, bullets, sounds);
            break;
        case ARIES + N_SIGNS:
            cast_aries_omega(player, bullets, sounds);
            break;
        case TAURUS:
            cast_taurus(player, grid, enemies, sounds);
            break;
        case TAURUS + N_SIGNS:
            cast_taurus_omega(player, grid, enemies, sounds);
            break;
        case GEMINI:
            cast_gemini(player, sounds);
            break;
        case GEMINI + N_SIGNS:
            cast_gemini_omega(player, sounds);
            break;
        case CANCER:
            cast_cancer(player, bullets, sounds);
            break;
        case CANCER + N_SIGNS:
            cast_cancer_omega(player, bullets, sounds);
            break;
        case LEO:
            cast_leo(player, sounds);
            break;
        case LEO + N_SIGNS:
            cast_leo_omega(player, sounds);
            break;
        case VIRGO:
            cast_virgo(player, grid, enemies, sounds);
            break;
        case VIRGO + N_SIGNS:
            cast_virgo_omega(player, grid, enemies, sounds);
            break;
        case LIBRA:
            cast_libra(player, bullets, sounds);
            break;
        case LIBRA + N_SIGNS:
            cast_libra_omega(player, bullets, sounds);
            break;
        case SCORPIO:
            cast_scorpio(player, sounds);
            break;
        case SCORPIO + N_SIGNS:
            cast_scorpio_omega(player, sounds);
            break;
        case SAGITTARIUS:
            cast_sagittarius(player, grid, sounds);
            break;
        case SAGITTARIUS + N_SIGNS:
            cast_sagittarius_omega(player, grid, sounds);
            break;
        case CAPRICORN:
            cast_capricorn(player, bullets, sounds);
            break;
        case CAPRICORN + N_SIGNS:
            cast_capricorn_omega(player, bullets, sounds);
            break;
        case AQUARIUS:
            cast_aquarius(player, sounds);
            break;
        case AQUARIUS + N_SIGNS:
            cast_aquarius_omega(player, sounds);
            break;
        case PISCES:
            cast_pisces(player, sounds);
            break;
        case PISCES + N_SIGNS:
            cast_pisces_omega(player, sounds);
            break;
        }
    }
    use_hex(player);
}

/**
 * Updates player every tick
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param glyphs The current state of the arena's glyphs
 * @param enemies The current state of the arena's enemies
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
void update_player(Player *player, Grid grid, Glyph glyphs[], Enemy enemies[], Bullet bullets[], uint16_t *score, const Sounds *sounds) {
    if (player->cooldown > 0) player->cooldown--;

    if (player->missile_delay > 0) player->missile_delay--;

    if (player->hit > 0) player->hit--;

    if (player->cooldown == 0) {
        switch (player->action) {
        case ACTION_WAIT:
            break;
        case ACTION_LEFT:
        case ACTION_RIGHT:
        case ACTION_UP:
        case ACTION_DOWN:
            move_player(player, glyphs, enemies, bullets, score, sounds);
            break;
        case ACTION_MISSILE:
            spawn_bullet(bullets, player->pos, BULLET_MISSILE, true);
            PlaySound(sounds->magic);
            break;
        case ACTION_HEX:
            cast_hex(player, grid, enemies, bullets, sounds);
            break;
        }

        for (Status status = 0; status < N_STATUSES; ++status) {
            if (player->statuses[status] && status != STATUS_LEECH && status != STATUS_REFLECT) {
                player->statuses[status]--;
            }
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

    if (get_tile_type(grid, player->pos) == TILE_MAGMA) {
        if (player->magma_cooldown > 0) player->magma_cooldown--;
        if (player->magma_cooldown == 0) {
            if (player->statuses[STATUS_PHANTOM]) {
                PlaySound(sounds->block);
            } else {
                if (player->hp > 0) player->hp--;
                PlaySound(sounds->hit);
            }
            player->magma_cooldown = 4;
        }
    } else {
        player->magma_cooldown = 0;
    }
}

/**
 * Updates the ID of the enemy whose health is currently being displayed
 * 
 * @param target_enemy The enemy ID to be changed
 * @param player The current state of the player
 * @param enemies The current state of the arena's enemies
 */
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

/**
 * Updates the player's time value each frame, used for lerp and animations
 * 
 * @param player The current state of the player
 * @param dt The delta time since last frame
 */
void update_player_time(Player *player, float dt) {
    player->t = min(player->t + dt / TICK_RATE / player->t_rate, 1.0f);
}
