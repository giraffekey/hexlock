#ifndef ENEMY_H
#define ENEMY_H

#include "util.h"

#define MAX_ENEMIES 9
#define MAX_ENEMY_STEPS 20

typedef enum {
    PIXIE,
    WISP,
    SCARAB,
    FLUFFY,
    MOLE,
    VENUS,
    N_ENEMIES,
} EnemyType;

typedef uint8_t EnemyTypeU8;

typedef enum {
    ENEMY_ACTION_WAIT,
    ENEMY_ACTION_SPAWN,
    ENEMY_ACTION_HIT,
    ENEMY_ACTION_DEATH,
    ENEMY_ACTION_PUSH,
    ENEMY_ACTION_LEFT,
    ENEMY_ACTION_RIGHT,
    ENEMY_ACTION_UP,
    ENEMY_ACTION_DOWN,
    ENEMY_ACTION_POWDER,
    ENEMY_ACTION_PELLET,
    ENEMY_ACTION_SPINY,
    ENEMY_ACTION_HIDE,
    ENEMY_ACTION_REVEAL,
    ENEMY_ACTION_SPIN,
    ENEMY_ACTION_REVERSE,
    ENEMY_ACTION_RETURN,
    ENEMY_ACTION_MELEE_ATTACK,
    ENEMY_ACTION_TELEPORT,
    ENEMY_ACTION_TELEPORT_TO_PLAYER,
} EnemyAction;

typedef uint8_t EnemyActionU8;

typedef struct {
    EnemyActionU8 action;
    uint8_t cooldown;
} EnemyStep;

typedef struct {
    EnemyStep steps[MAX_ENEMY_STEPS];
    size_t index;
    size_t size;
} EnemyPlan;

typedef struct {} EnemyNoData;

typedef struct {
    uint8_t orient;
} EnemyScarabData;

typedef struct {
    bool is_moving_up;
    Position origin;
} EnemyFluffyData;

typedef struct {
    bool hidden;
} EnemyMoleData;

typedef union {
    EnemyNoData none;
    EnemyScarabData scarab;
    EnemyFluffyData fluffy;
    EnemyMoleData mole;
} EnemyData;

typedef struct {
    bool exists;
    EnemyTypeU8 type;
    Position pos;
    float t;
    float t_rate;
    EnemyPlan plan;
    uint8_t cooldown;
    uint8_t hit;
    uint8_t magma_cooldown;
    uint8_t max_hp;
    uint8_t hp;
    EnemyData data;
} Enemy;

static const uint8_t MAX_ENEMY_HEALTH[N_ENEMIES] = {4, 3, 2, 6, 4, 6};

static const uint16_t ENEMY_SCORES[N_ENEMIES] = {20, 30, 30, 10, 30, 10};

void add_step(EnemyPlan *plan, EnemyActionU8 action, uint8_t cooldown);

EnemyStep current_step(const Enemy *enemy);

EnemyAction current_action(const Enemy *enemy);

bool is_enemy_dead(const Enemy *enemy);

bool can_be_hit(const Enemy *enemy);

int current_enemy_frame(const Enemy *enemy);

void hit_enemy(Enemy *enemy);

void kill_enemy(Enemy *enemy);

#endif
