#ifndef PLAYER_H
#define PLAYER_H

#include "util.h"
#include "glyph.h"

#define MAX_HEXES 10
#define MAX_PLAYER_HEALTH 10

typedef enum {
    ACTION_WAIT,
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_UP,
    ACTION_DOWN,
    ACTION_MISSILE,
    ACTION_HEX,
} Action;

typedef uint8_t ActionU8;

typedef struct {
    bool valid;
    SignU8 sign;
    bool omega;
} Hex;

typedef enum {
    STATUS_MOVE_FAST,
    STATUS_CAST_FAST,
    STATUS_FIRE_CLOAK,
    STATUS_WILDFIRE_CLOAK,
    STATUS_LEECH,
    STATUS_REFLECT,
    STATUS_PHANTOM,
    N_STATUSES,
} Status;

typedef struct {
    Position pos;
    float t;
    ActionU8 action;
    uint8_t cooldown;
    float t_rate;
    ActionU8 next_action;
    uint8_t next_cooldown;
    float next_t_rate;
    uint8_t wait_frame;
    uint8_t missile_delay;
    uint8_t hp;
    SignU8 signs[MAX_SIGNS];
    size_t n_signs;
    Hex hexes[MAX_HEXES];
    size_t n_hexes;
    uint16_t statuses[N_STATUSES];
} Player;

const char* get_hex_name(Hex hex);

const char* get_hex_desc(Hex hex);

Hex find_hex(SignU8 signs[]);

void add_hex(Player *player, Hex hex);

void use_hex(Player *player);

void damage_player(Player *player, uint8_t damage);

#endif
