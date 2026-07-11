#include "enemy.h"

static void find_used(bool is_used[], const Grid grid, const Enemy enemies[], bool only_player_tile, bool include_next_pos, size_t enemy_limit) {
    for (uint8_t y = 0; y < N_ROWS; ++y) {
        for (uint8_t x = 0; x < N_COLS; ++x) {
            Position pos = {x, y};
            if (is_wall(grid, pos) || (only_player_tile && is_player_tile(grid, pos))) {
                is_used[y * 6 + x] = true;
            }
        }
    }

    for (size_t i = 0; i < enemy_limit; ++i) {
        const Enemy *enemy = &enemies[i];
        if (enemy->exists) {
            is_used[enemy->pos.y * 6 + enemy->pos.x] = true;
            if (include_next_pos) {
                switch (current_action(enemy)) {
                case ENEMY_ACTION_LEFT:
                    is_used[enemy->pos.y * 6 + (enemy->pos.x - 1)] = true;
                    break;
                case ENEMY_ACTION_RIGHT:
                    is_used[enemy->pos.y * 6 + (enemy->pos.x + 1)] = true;
                    break;
                case ENEMY_ACTION_UP:
                    is_used[(enemy->pos.y - 1) * 6 + enemy->pos.x] = true;
                    break;
                case ENEMY_ACTION_DOWN:
                    is_used[(enemy->pos.y + 1) * 6 + enemy->pos.x] = true;
                    break;
                }
            }
        }
    }
}

static bool any_available(bool is_used[]) {
    for (size_t i = 0; i < 18; ++i) {
        if (!is_used[i]) {
            return true;
        }
    }
    return false;
}

static EnemyPlan create_pixie_plan(const Enemy *enemy, const Grid grid, const Enemy enemies[]) {
    bool is_used[18] = {0};
    find_used(is_used, grid, enemies, true, false, MAX_ENEMIES);

    uint8_t x = enemy->pos.x;
    uint8_t y = enemy->pos.y;

    const uint8_t next[2][3] = {{1, 0, 0}, {2, 2, 1}};
    bool is_used_0 = is_used[next[0][y] * 6 + x];
    bool is_used_1 = is_used[next[1][y] * 6 + x];

    uint8_t next_y;
    if (is_used_0 && is_used_1) {
        next_y = enemy->pos.y;
    } else if (is_used_0) {
        next_y = next[1][y];
    } else if (is_used_1) {
        next_y = next[0][y];
    } else {
        next_y = next[rand() % 2][y];
    }

    EnemyPlan plan = {0};

    if (next_y < y) {
        for (uint8_t i = 0; i < y - next_y; i++) {
            add_step(&plan, ENEMY_ACTION_UP, 2);
            add_step(&plan, ENEMY_ACTION_WAIT, 2);
        }
    } else if (next_y > y) {
        for (uint8_t i = 0; i < next_y - y; i++) {
            add_step(&plan, ENEMY_ACTION_DOWN, 2);
            add_step(&plan, ENEMY_ACTION_WAIT, 2);
        }
    }

    add_step(&plan, ENEMY_ACTION_WAIT, 2);
    add_step(&plan, ENEMY_ACTION_POWDER, 2);
    add_step(&plan, ENEMY_ACTION_WAIT, 8);
    add_step(&plan, ENEMY_ACTION_POWDER, 2);
    add_step(&plan, ENEMY_ACTION_WAIT, 4);

    return plan;
}

static EnemyPlan create_wisp_plan(const Enemy *enemy) {
    EnemyPlan plan = {0};

    for (int i = 0; i < 3; ++i) {
        add_step(&plan, ENEMY_ACTION_WAIT, 4);
        add_step(&plan, ENEMY_ACTION_TELEPORT, 2);
    }

    add_step(&plan, ENEMY_ACTION_WAIT, 4);
    add_step(&plan, ENEMY_ACTION_TELEPORT_TO_PLAYER, 2);
    add_step(&plan, ENEMY_ACTION_WAIT, 4);
    add_step(&plan, ENEMY_ACTION_MELEE_ATTACK, 2);

    return plan;
}

static EnemyPlan create_scarab_plan(const Enemy *enemy, const Grid grid, const Enemy enemies[]) {
    bool is_used[18] = {0};
    find_used(is_used, grid, enemies, false, false, MAX_ENEMIES);

    EnemyPlan plan = {0};
    
    uint8_t spins = rand() % 4 + 4;
    uint8_t max_moves = 0;
    EnemyAction action;

    for (int i = 0; i < 4; ++i) {
        spins++;
        uint8_t next_orient = (enemy->data.scarab.orient + spins) % 4;

        switch (next_orient) {
        case 0:
            if (enemy->pos.x < N_COLS - 1) {
                for (uint8_t x = enemy->pos.x + 1; x < N_COLS; ++x) {
                    if (is_used[enemy->pos.y * 6 + x]) break;
                    max_moves++;
                }
            }
            action = ENEMY_ACTION_RIGHT;
            break;
        case 1:
            if (enemy->pos.y < N_ROWS - 1) {
                for (uint8_t y = enemy->pos.y + 1; y < N_ROWS; ++y) {
                    if (is_used[y * 6 + enemy->pos.x]) break;
                    max_moves++;
                }
            }
            action = ENEMY_ACTION_DOWN;
            break;
        case 2:
            if (enemy->pos.x > 0) {
                for (uint8_t x = enemy->pos.x; x > 0; --x) {
                    if (is_used[enemy->pos.y * 6 + (x - 1)]) break;
                    max_moves++;
                }
            }
            action = ENEMY_ACTION_LEFT;
            break;
        case 3:
            if (enemy->pos.y > 0) {
                for (uint8_t y = enemy->pos.y; y > 0; --y) {
                    if (is_used[(y - 1) * 6 + enemy->pos.x]) break;
                    max_moves++;
                }
            }
            action = ENEMY_ACTION_UP;
            break;
        }

        if (max_moves > 0) break;
    }

    for (uint8_t i = 0; i < spins; ++i) {
        add_step(&plan, ENEMY_ACTION_SPIN, 1);
    }

    if (max_moves > 0) {
        uint8_t moves = max_moves == 1 ? 1 : 2;
        for (uint8_t i = 0; i < moves; ++i) add_step(&plan, action, 2);
    }

    return plan;
}

static EnemyPlan create_fluffy_plan(const Enemy *enemy, const Grid grid, const Enemy enemies[], const Player *player) {
    bool is_used[18] = {0};
    find_used(is_used, grid, enemies, true, false, MAX_ENEMIES);

    bool can_see_player = player->pos.x < enemy->pos.x && player->pos.y == enemy->pos.y;
    if (can_see_player) {
        for (uint8_t x = player->pos.x + 1; x < enemy->pos.x; ++x) {
            Position pos = {x, enemy->pos.y};
            if (is_wall(grid, pos)) {
                can_see_player = false;
                break;
            }
        }
    }

    EnemyPlan plan = {0};
    
    if (can_see_player) {
        for (int i = 0; i < enemy->pos.x; ++i) {
            add_step(&plan, ENEMY_ACTION_LEFT, 1);
        }
        add_step(&plan, ENEMY_ACTION_RETURN, enemy->pos.x);
    }

    if (enemy->data.fluffy.is_moving_up) {
        if (is_used[(enemy->pos.y - 1) * 6 + enemy->pos.x]) {
            add_step(&plan, ENEMY_ACTION_REVERSE, 2);
        } else {
            add_step(&plan, ENEMY_ACTION_WAIT, 4);
            add_step(&plan, ENEMY_ACTION_UP, 2);
        }
    } else {
        if (is_used[(enemy->pos.y + 1) * 6 + enemy->pos.x]) {
            add_step(&plan, ENEMY_ACTION_REVERSE, 2);
        } else {
            add_step(&plan, ENEMY_ACTION_WAIT, 4);
            add_step(&plan, ENEMY_ACTION_DOWN, 2);
        }
    }
    
    return plan;
}

static EnemyPlan create_mole_plan(const Enemy *enemy, const Grid grid, const Enemy enemies[]) {
    bool is_used[18] = {0};
    find_used(is_used, grid, enemies, true, false, MAX_ENEMIES);
    bool has_available = any_available(is_used);

    EnemyPlan plan = {0};

    if (!has_available) {
        add_step(&plan, ENEMY_ACTION_WAIT, 4);
        for (int i = 0; i < 3; ++i) {
            add_step(&plan, ENEMY_ACTION_PELLET, 2);
            add_step(&plan, ENEMY_ACTION_WAIT, 2);
        }
        return plan;
    }

    add_step(&plan, ENEMY_ACTION_HIDE, 1);

    Position next_pos;
    do {
        next_pos.x = rand() % 6;
        next_pos.y = rand() % 3;
    } while (is_used[next_pos.y * 6 + next_pos.x]);

    Position pos = enemy->pos;
    while (!is_pos_eq(pos, next_pos)) {
        int8_t dx = (int8_t)next_pos.x - (int8_t)pos.x;
        int8_t dy = (int8_t)next_pos.y - (int8_t)pos.y;
        int8_t move_x = (dx != 0) && (dy == 0 || rand() % 2 == 0);
        
        if (move_x) {
            int step_x = (dx > 0) - (dx < 0);
            pos.x += step_x;
            EnemyActionU8 action = step_x > 0 ? ENEMY_ACTION_RIGHT : ENEMY_ACTION_LEFT;
            add_step(&plan, action, 1);
        } else {
            int step_y = (dy > 0) - (dy < 0);
            pos.y += step_y;
            EnemyActionU8 action = step_y > 0 ? ENEMY_ACTION_DOWN : ENEMY_ACTION_UP;
            add_step(&plan, action, 1);
        }
    }

    add_step(&plan, ENEMY_ACTION_REVEAL, 1);
    add_step(&plan, ENEMY_ACTION_WAIT, 4);

    for (int i = 0; i < 3; ++i) {
        add_step(&plan, ENEMY_ACTION_PELLET, 2);
        add_step(&plan, ENEMY_ACTION_WAIT, 2);
    }

    return plan;
}

static EnemyPlan create_venus_plan(const Enemy *enemy, const Player *player) {
    EnemyPlan plan = {0};
    if (player->pos.x < enemy->pos.x && player->pos.y == enemy->pos.y) {
        add_step(&plan, ENEMY_ACTION_SPINY, 2);
        add_step(&plan, ENEMY_ACTION_WAIT, 6);
    } else {
        add_step(&plan, ENEMY_ACTION_WAIT, 2);
    }
    return plan;
}

static EnemyPlan create_enemy_plan(const Enemy *enemy, const Grid grid, const Enemy enemies[], const Player *player) {
    switch (enemy->type) {
    case PIXIE: return create_pixie_plan(enemy, grid, enemies);
    case WISP: return create_wisp_plan(enemy);
    case SCARAB: return create_scarab_plan(enemy, grid, enemies);
    case FLUFFY: return create_fluffy_plan(enemy, grid, enemies, player);
    case MOLE: return create_mole_plan(enemy, grid, enemies);
    case VENUS: return create_venus_plan(enemy, player);
    }
    return (EnemyPlan){0};
}

static void move_enemy(Enemy *enemy, EnemyActionU8 dir, Player *player, Bullet bullets[]) {
    switch (dir) {
    case ENEMY_ACTION_LEFT:
        enemy->pos.x--;
        break;
    case ENEMY_ACTION_RIGHT:
        enemy->pos.x++;
        break;
    case ENEMY_ACTION_UP:
        enemy->pos.y--;
        if (enemy->type == FLUFFY) enemy->data.fluffy.origin = enemy->pos;
        break;
    case ENEMY_ACTION_DOWN:
        enemy->pos.y++;
        if (enemy->type == FLUFFY) enemy->data.fluffy.origin = enemy->pos;
        break;
    }

    check_enemy_player_collision(enemy, player);
    check_enemy_bullet_collision(enemy, bullets);
}

static void hide_enemy(Enemy *enemy) {
    enemy->data.mole.hidden = true;
}

static void reveal_enemy(Enemy *enemy, size_t id, const Enemy enemies[]) {
    bool is_available = true;
    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        if (i == id) continue;
        const Enemy *other = &enemies[i];
        if (other->exists && is_pos_eq(enemy->pos, other->pos)) {
            is_available = false;
            break;
        }
    }

    if (is_available) {
        enemy->data.mole.hidden = false;
    } else {
        enemy->plan.index = enemy->plan.size;
    }
}

static void spin_enemy(Enemy *enemy) {
    EnemyScarabData *data = &enemy->data.scarab;
    data->orient = (data->orient + 1) % 4;
}

static void reverse_enemy(Enemy *enemy) {
    switch (enemy->type) {
    case FLUFFY:
        bool *is_moving_up = &enemy->data.fluffy.is_moving_up;
        *is_moving_up = !*is_moving_up;
        break;
    }
}

static void return_enemy(Enemy *enemy) {
    switch (enemy->type) {
    case FLUFFY:
        enemy->pos = enemy->data.fluffy.origin;
        break;
    }
}

static void melee_attack(Enemy *enemy, Player *player, Bullet bullets[]) {
    Position attack_pos = {enemy->pos.x - 1, enemy->pos.y};
    if (is_pos_eq(player->pos, attack_pos)) {
        switch (enemy->type) {
        case WISP:
            damage_player(player, 2);
            break;
        }
    }
    
    if (player->statuses[STATUS_WILDFIRE_CLOAK]) {
        spawn_bullet(bullets, player->pos, BULLET_WILDFIRE, true);
    } else if (player->statuses[STATUS_FIRE_CLOAK]) {
        spawn_bullet(bullets, player->pos, BULLET_FIREBALL, true);
    }
}

static void teleport_enemy(Enemy *enemy, const Grid grid, const Enemy enemies[]) {
    bool is_used[18] = {0};
    find_used(is_used, grid, enemies, true, false, MAX_ENEMIES);
    bool has_available = any_available(is_used);

    if (!has_available) return;

    Position pos;
    do {
        pos.x = rand() % 6;
        pos.y = rand() % 3;
    } while (is_used[pos.y * 6 + pos.x]);
    enemy->pos = pos;
}

static void teleport_to_player(Enemy *enemy, const Grid grid, Player *player) {
    Position pos = {player->pos.x + 1, player->pos.y};
    if (is_wall(grid, pos)) enemy->plan = (EnemyPlan){{ENEMY_ACTION_WAIT, 1}, 0, 1};
    else enemy->pos = pos;
}

static void execute_enemy_plan(Enemy *enemy, size_t id, const Grid grid, const Enemy enemies[], Player *player, Bullet bullets[]) {
    EnemyStep step = current_step(enemy);
    switch (step.action) {
    case ENEMY_ACTION_WAIT:
        break;
    case ENEMY_ACTION_LEFT:
    case ENEMY_ACTION_RIGHT:
    case ENEMY_ACTION_UP:
    case ENEMY_ACTION_DOWN:
        move_enemy(enemy, step.action, player, bullets);
        break;
    case ENEMY_ACTION_POWDER:
        spawn_bullet(bullets, enemy->pos, BULLET_POWDER, false);
        break;
    case ENEMY_ACTION_PELLET:
        spawn_bullet(bullets, enemy->pos, BULLET_PELLET, false);
        break;
    case ENEMY_ACTION_SPINY:
        spawn_bullet(bullets, enemy->pos, BULLET_SPINY, false);
        break;
    case ENEMY_ACTION_HIDE:
        hide_enemy(enemy);
        break;
    case ENEMY_ACTION_REVEAL:
        reveal_enemy(enemy, id, enemies);
        break;
    case ENEMY_ACTION_SPIN:
        spin_enemy(enemy);
        break;
    case ENEMY_ACTION_REVERSE:
        reverse_enemy(enemy);
        break;
    case ENEMY_ACTION_RETURN:
        return_enemy(enemy);
        break;
    case ENEMY_ACTION_MELEE_ATTACK:
        melee_attack(enemy, player, bullets);
        break;
    case ENEMY_ACTION_TELEPORT:
        teleport_enemy(enemy, grid, enemies);
        break;
    case ENEMY_ACTION_TELEPORT_TO_PLAYER:
        teleport_to_player(enemy, grid, player);
        break;
    }
    enemy->plan.index++;
}

static bool next_pos_taken(Enemy *enemy, size_t id, const Grid grid, const Enemy enemies[]) {
    bool is_used[18] = {0};
    switch (enemy->type) {
    case PIXIE:
        find_used(is_used, grid, enemies, true, true, id);
        break;
    case SCARAB:
    case FLUFFY:
        find_used(is_used, grid, enemies, false, true, id);
        break;
    default:
        return false;
    }

    switch (current_action(enemy)) {
    case ENEMY_ACTION_LEFT:
        if (enemy->type == FLUFFY) return false;
        return is_used[enemy->pos.y * 6 + (enemy->pos.x - 1)];
        break;
    case ENEMY_ACTION_RIGHT:
        return is_used[enemy->pos.y * 6 + (enemy->pos.x + 1)];
        break;
    case ENEMY_ACTION_UP:
        return is_used[(enemy->pos.y - 1) * 6 + enemy->pos.x];
        break;
    case ENEMY_ACTION_DOWN:
        return is_used[(enemy->pos.y + 1) * 6 + enemy->pos.x];
        break;
    }
    return false;
}

static void update_enemy(Enemy *enemy, size_t id, const Grid grid, const Enemy enemies[], Player *player, Bullet bullets[]) {
    if (enemy->cooldown > 0) enemy->cooldown--;

    if (enemy->cooldown == 0) {
        if (enemy->type == FLUFFY) {
            bool *is_moving_up = &enemy->data.fluffy.is_moving_up;
            if (*is_moving_up && enemy->pos.y == 0) *is_moving_up = false;
            else if (!*is_moving_up && enemy->pos.y == 2) *is_moving_up = true;
        }

        if (enemy->plan.index < enemy->plan.size) {
            execute_enemy_plan(enemy, id, grid, enemies, player, bullets);
        } else {
            enemy->plan = create_enemy_plan(enemy, grid, enemies, player);
        }

        if (get_tile_type(grid, enemy->pos) == TILE_MAGMA) {
            if (enemy->magma_cooldown > 0) enemy->magma_cooldown--;
            if (enemy->magma_cooldown == 0) {
                enemy->hp--;
                if (enemy->hp == 0) enemy->exists = false;
                enemy->magma_cooldown = 4;
            }
        } else {
            enemy->magma_cooldown = 0;
        }

        uint8_t cooldown = current_step(enemy).cooldown;
        enemy->cooldown = cooldown;
        enemy->t_rate = (float)cooldown;
        enemy->t = 0.0f;

        if (next_pos_taken(enemy, id, grid, enemies)) enemy->plan = create_enemy_plan(enemy, grid, enemies, player);
    }
}

void update_enemies(Enemy enemies[], const Grid grid, Player *player, Bullet bullets[]) {
    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        Enemy *enemy = &enemies[i];
        if (enemy->exists) update_enemy(enemy, i, grid, enemies, player, bullets);
    }
}

void update_enemies_time(Enemy enemies[], float dt) {
    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        Enemy *enemy = &enemies[i];
        if (enemy->exists) {
            enemy->t = min(enemy->t + dt / TICK_RATE / enemy->t_rate, 1.0f);
        }
    }
}
