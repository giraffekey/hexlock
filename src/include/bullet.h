#ifndef BULLET_H
#define BULLET_H

#include "util.h"

#define MAX_BULLETS 80

typedef enum {
    BULLET_MISSILE,
    BULLET_FIREBALL,
    BULLET_WILDFIRE,
    BULLET_FIRE_WAVE,
    BULLET_FIRE_BLAST,
    BULLET_FLAME,
    BULLET_WHIRLPOOL,
    BULLET_HURRICANE,
    BULLET_TWISTER,
    BULLET_SHOCKWAVE,
    BULLET_POWDER,
    BULLET_PELLET,
    BULLET_SPINY,
    N_BULLETS,
} BulletType;

typedef uint8_t BulletTypeU8;

typedef struct {} BulletNoData;

typedef struct {
    bool is_moving;
    uint8_t lifetime;
    bool is_child;
} BulletWhirlpoolData;

typedef struct {
    bool is_moving_up;
    uint8_t cooldown;
} BulletTwisterData;

typedef union {
    BulletNoData none;
    BulletWhirlpoolData whirlpool;
    BulletTwisterData twister;
} BulletData;

typedef struct {
    bool exists;
    BulletTypeU8 type;
    Position pos;
    float t;
    bool is_player;
    BulletData data;
} Bullet;

uint8_t get_bullet_damage(BulletTypeU8 type);

void spawn_bullet_with_data(Bullet bullets[], Position pos, BulletType type, bool is_player, BulletData data);

void spawn_bullet(Bullet bullets[], Position pos, BulletType type, bool is_player);

void reverse_bullet(Bullet *bullet);

#endif
