#include "collide.h"

void check_player_glyph_collision(Player *player, Glyph glyphs[]) {
    for (size_t i = 0; i < MAX_GLYPHS; ++i) {
        Glyph *glyph = &glyphs[i];
        if (glyph->exists && is_pos_eq(player->pos, glyph->pos)) {
            player->signs[player->n_signs % MAX_SIGNS] = glyph->sign;
            player->n_signs = player->n_signs % MAX_SIGNS + 1;

            if (player->n_signs == MAX_SIGNS) {
                Hex hex = find_hex(player->signs);
                if (hex.valid) add_hex(player, hex);
            }

            glyph->exists = false;
            break;
        }
    }
}

static void force_return(Enemy *enemy) {
    if (current_action(enemy) == ENEMY_ACTION_LEFT) {
        EnemyPlan plan = {0};
        add_step(&plan, ENEMY_ACTION_WAIT, 0);
        add_step(&plan, ENEMY_ACTION_RETURN, enemy->data.fluffy.origin.x - enemy->pos.x);

        if (enemy->data.fluffy.is_moving_up) {
            add_step(&plan, ENEMY_ACTION_WAIT, 2);
            add_step(&plan, ENEMY_ACTION_UP, 2);
        } else {
            add_step(&plan, ENEMY_ACTION_WAIT, 2);
            add_step(&plan, ENEMY_ACTION_DOWN, 2);
        }

        enemy->plan = plan;
    }
}

static void on_player_enemy_collision(Player *player, Enemy *enemy) {
    switch (enemy->type) {
    case SCARAB:
        damage_player(player, 2);
        break;
    case FLUFFY:
        if (current_action(enemy) != ENEMY_ACTION_RETURN) {
            damage_player(player, 3);
            force_return(enemy);
        }
        break;
    }
}

void check_player_enemy_collision(Player *player, Enemy enemies[]) {
    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        Enemy *enemy = &enemies[i];
        if (enemy->exists && is_pos_eq(player->pos, enemy->pos)) on_player_enemy_collision(player, enemy);
    }
}

void check_enemy_player_collision(Enemy *enemy, Player *player) {
    if (is_pos_eq(enemy->pos, player->pos)) on_player_enemy_collision(player, enemy);
}

static void on_bullet_player_collision(Bullet *bullet, Player *player, Bullet bullets[]) {
    uint8_t damage = get_bullet_damage(bullet->type);

    bool immune = player->statuses[STATUS_PHANTOM];

    if (player->statuses[STATUS_LEECH]) {
        player->hp = min(player->hp + damage, MAX_PLAYER_HEALTH);
        player->statuses[STATUS_LEECH]--;
        immune = true;
    }

    if (player->statuses[STATUS_REFLECT]) {
        reverse_bullet(bullet);
        player->statuses[STATUS_REFLECT]--;
        immune = true;
    } else {
        switch (bullet->type) {
        case BULLET_WHIRLPOOL:
        case BULLET_HURRICANE:
        case BULLET_TWISTER:
        case BULLET_SHOCKWAVE:
            break;
        default:
            bullet->exists = false;
            break;
        }
    }

    if (!immune) {
        damage_player(player, damage);
    }

    if (player->statuses[STATUS_WILDFIRE_CLOAK]) {
        spawn_bullet(bullets, player->pos, BULLET_WILDFIRE, true);
    } else if (player->statuses[STATUS_FIRE_CLOAK]) {
        spawn_bullet(bullets, player->pos, BULLET_FIREBALL, true);
    }
}

static void on_bullet_enemy_collision(Bullet *bullet, Enemy *enemy) {
    if (can_be_hit(enemy)) {
        uint8_t damage = get_bullet_damage(bullet->type);
        if (damage >= enemy->hp) enemy->exists = false;
        else enemy->hp -= damage;
    }

    bool remove_bullet = true;

    switch (enemy->type) {
    case SCARAB:
        if (current_action(enemy) == ENEMY_ACTION_SPIN) {
            reverse_bullet(bullet);
            remove_bullet = false;
        }
        break;
    case FLUFFY:
        force_return(enemy);
        break;
    }

    switch (bullet->type) {
    case BULLET_WHIRLPOOL:
    case BULLET_HURRICANE:
    case BULLET_TWISTER:
    case BULLET_SHOCKWAVE:
        remove_bullet = false;
        break;
    }

    if (remove_bullet) bullet->exists = false;
}

static void on_bullet_collision(Bullet *bullet, Bullet bullets[]) {
    switch (bullet->type) {
    case BULLET_FIRE_WAVE:
        {
            Position positions[2] = {
                {bullet->pos.x + 1, bullet->pos.y - 1},
                {bullet->pos.x + 1, bullet->pos.y + 1},
            };
            for (size_t i = 0; i < 2; ++i) {
                Position pos = positions[i];
                if (is_in_bounds(pos)) spawn_bullet(bullets, pos, BULLET_FLAME, bullet->is_player);
            }
        }
        break;
    case BULLET_FIRE_BLAST:
        {
            Position positions[4] = {
                {bullet->pos.x + 1, bullet->pos.y - 1},
                {bullet->pos.x + 1, bullet->pos.y + 1},
                {bullet->pos.x - 1, bullet->pos.y - 1},
                {bullet->pos.x - 1, bullet->pos.y + 1},
            };
            for (size_t i = 0; i < 4; ++i) {
                Position pos = positions[i];
                if (is_in_bounds(pos)) spawn_bullet(bullets, pos, BULLET_FLAME, bullet->is_player);
            }
        }
        break;
    case BULLET_WHIRLPOOL:
        {
            BulletWhirlpoolData *data = &bullet->data.whirlpool;
            if (data->is_moving) {
                data->is_moving = false;
                Position positions[4] = {
                    {bullet->pos.x - 1, bullet->pos.y},
                    {bullet->pos.x + 1, bullet->pos.y},
                    {bullet->pos.x, bullet->pos.y - 1},
                    {bullet->pos.x, bullet->pos.y + 1},
                };
                for (size_t i = 0; i < 4; ++i) {
                    Position pos = positions[i];
                    if (is_in_bounds(pos)) {
                        BulletData spawn_data = bullet->data;
                        spawn_data.whirlpool.lifetime++;
                        spawn_bullet_with_data(bullets, pos, BULLET_WHIRLPOOL, bullet->is_player, spawn_data);
                    }
                }
            }
        }
        break;
    case BULLET_HURRICANE:
        {
            BulletWhirlpoolData *data = &bullet->data.whirlpool;
            if (data->is_moving) {
                data->is_moving = false;
                Position positions[8] = {
                    {bullet->pos.x - 1, bullet->pos.y},
                    {bullet->pos.x + 1, bullet->pos.y},
                    {bullet->pos.x, bullet->pos.y - 1},
                    {bullet->pos.x, bullet->pos.y + 1},
                    {bullet->pos.x - 1, bullet->pos.y - 1},
                    {bullet->pos.x + 1, bullet->pos.y - 1},
                    {bullet->pos.x - 1, bullet->pos.y + 1},
                    {bullet->pos.x + 1, bullet->pos.y + 1},
                };
                for (size_t i = 0; i < 8; ++i) {
                    Position pos = positions[i];
                    if (is_in_bounds(pos)) {
                        BulletData spawn_data = bullet->data;
                        spawn_data.whirlpool.lifetime++;
                        spawn_bullet_with_data(bullets, pos, BULLET_HURRICANE, bullet->is_player, spawn_data);
                    }
                }
            }
        }
        break;
    }
}

void check_bullet_player_collision(Bullet *bullet, Player *player, Bullet bullets[]) {
    if (is_pos_eq(bullet->pos, player->pos)) {
        on_bullet_player_collision(bullet, player, bullets);
        on_bullet_collision(bullet, bullets);
    }
}

void check_player_bullet_collision(Player *player, Bullet bullets[]) {
    for (size_t i = 0; i < MAX_BULLETS; ++i) {
        Bullet *bullet = &bullets[i];
        if (bullet->exists && is_pos_eq(player->pos, bullet->pos)) {
            on_bullet_player_collision(bullet, player, bullets);
            on_bullet_collision(bullet, bullets);
            break;
        }
    }
}

void check_bullet_enemy_collision(Bullet *bullet, Enemy enemies[], Bullet bullets[]) {
    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        Enemy *enemy = &enemies[i];
        if (enemy->exists && is_pos_eq(bullet->pos, enemy->pos)) {
            on_bullet_enemy_collision(bullet, enemy);
            on_bullet_collision(bullet, bullets);
            break;
        }
    }
}

void check_enemy_bullet_collision(Enemy *enemy, Bullet bullets[]) {
    for (size_t i = 0; i < MAX_BULLETS; ++i) {
        Bullet *bullet = &bullets[i];
        if (bullet->exists && is_pos_eq(enemy->pos, bullet->pos)) {
            on_bullet_enemy_collision(bullet, enemy);
            on_bullet_collision(bullet, bullets);
            break;
        }
    }
}
