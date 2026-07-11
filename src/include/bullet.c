#include "bullet.h"

uint8_t get_bullet_damage(BulletTypeU8 type) {
	const uint8_t DAMAGE[N_BULLETS] = {1, 2, 3, 4, 5, 3, 1, 1, 2, 3, 1, 1, 2};
	return DAMAGE[type];
}

void spawn_bullet_with_data(Bullet bullets[], Position pos, BulletType type, bool is_player, BulletData data) {
    for (size_t i = 0; i < MAX_BULLETS; ++i) {
        Bullet *bullet = &bullets[i];
        if (!bullets[i].exists) {
            bullets[i] = (Bullet){true, type, pos, 0.0f, is_player, data};
            break;
        }
    }
}

void spawn_bullet(Bullet bullets[], Position pos, BulletType type, bool is_player) {
    BulletData data;
    data.none = (BulletNoData){};
    spawn_bullet_with_data(bullets, pos, type, is_player, data);
}

void reverse_bullet(Bullet *bullet) {
    if (bullet->type == BULLET_TWISTER) {
        BulletTwisterData *data = &bullet->data.twister;
        if (data->cooldown == 2) bullet->is_player = !bullet->is_player;
    } else {
        bullet->is_player = !bullet->is_player;
    }
}
