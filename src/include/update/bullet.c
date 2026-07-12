#include "bullet.h"

static bool update_bullet_position(Bullet *bullet) {
	if (bullet->is_player) {
        if (bullet->pos.x < 5) bullet->pos.x++;
        else return false;
    } else {
        if (bullet->pos.x > 0) bullet->pos.x--;
        else return false;
    }
    return true;
}

static bool update_twister(Bullet *bullet) {
	BulletTwisterData *data = &bullet->data.twister;
	bool active = true;

    if (data->cooldown > 0) data->cooldown--;

    if (data->cooldown == 0) {
        if (!update_bullet_position(bullet)) active = false;

        if (data->is_moving_up) {
            if (bullet->pos.y > 0) bullet->pos.y--;
            if (bullet->pos.y == 0) data->is_moving_up = false;
        } else {
            if (bullet->pos.y < 2) bullet->pos.y++;
            if (bullet->pos.y == 2) data->is_moving_up = true;
        }

        bullet->t = 0.0f;
        data->cooldown = 2;
    }

    return active;
}

static bool update_whirlpool(Bullet *bullet) {
	BulletWhirlpoolData *data = &bullet->data.whirlpool;
	bool active = true;

    if (data->is_moving) {
        if (!update_bullet_position(bullet)) active = false;
    } else {
        data->lifetime--;
        if (data->lifetime == 0) active = false;
    }
    bullet->t = 0.0f;

    return active;
}

static void hit_wall(Bullet *bullet, Grid grid) {
	switch (get_tile_type(grid, bullet->pos)) {
    case TILE_ROCK:
	    damage_tile(grid, bullet->pos, get_bullet_damage(bullet->type));
	    bullet->exists = false;
	    break;
	case TILE_CRYSTAL:
	    damage_tile(grid, bullet->pos, get_bullet_damage(bullet->type));
	    reverse_bullet(bullet);
	    break;
	}
}

static void update_bullet(Bullet *bullet, Grid grid, Player *player, Enemy enemies[], Bullet spawned[], const Sounds *sounds) {
    bool active = true;
    switch (bullet->type) {
    case BULLET_TWISTER:
        active = update_twister(bullet);
        break;
    case BULLET_FLAME:
        break;
    case BULLET_WHIRLPOOL:
    case BULLET_HURRICANE:
        active = update_whirlpool(bullet);
        break;
    default:
        active = update_bullet_position(bullet);
        bullet->t = 0.0f;
        break;
    }

    if (active) {
        if (is_wall(grid, bullet->pos)) {
            hit_wall(bullet, grid);
        } else {
            if (bullet->is_player) check_bullet_enemy_collision(bullet, enemies, spawned, sounds);
            else check_bullet_player_collision(bullet, player, spawned, sounds);
        }
    } else {
        bullet->exists = false;
    }

    if (bullet->type == BULLET_FLAME) bullet->exists = false;
}

void update_bullets(Bullet bullets[], Grid grid, Player *player, Enemy enemies[], const Sounds *sounds) {
    Bullet spawned[MAX_BULLETS] = {0};

    for (size_t i = 0; i < MAX_BULLETS; ++i) {
        Bullet *bullet = &bullets[i];
        if (bullet->exists) update_bullet(bullet, grid, player, enemies, spawned, sounds);
    }

    size_t start = 0;
    for (size_t i = 0; i < MAX_BULLETS; ++i) {
        if (!spawned[i].exists) break;
        for (size_t j = start; j < MAX_BULLETS; ++j) {
            if (!bullets[j].exists) {
                bullets[j] = spawned[i];
                start = j + 1;
                break;
            }
        }
    }
}

void update_bullets_time(Bullet bullets[], float dt) {
    for (size_t i = 0; i < MAX_BULLETS; ++i) {
        Bullet *bullet = &bullets[i];
        if (bullet->exists) {
            switch (bullet->type) {
            case BULLET_FLAME:
                break;
            case BULLET_WHIRLPOOL:
            case BULLET_HURRICANE:
                BulletWhirlpoolData *data = &bullet->data.whirlpool;
                if (data->is_moving) bullet->t = min(bullet->t + dt / TICK_RATE, 1.0f);
                break;
            case BULLET_TWISTER:
                bullet->t = min(bullet->t + dt / TICK_RATE / 2.0f, 1.0f);
                break;
            default:
                bullet->t = min(bullet->t + dt / TICK_RATE, 1.0f);
                break;
            }
        }
    }
}
