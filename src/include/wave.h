#ifndef WAVE_H
#define WAVE_H

#include "enemy.h"

#define N_DIFFICULTY_LEVELS 5
#define N_WAVES_PER_DIFFICULTY 10

static const EnemyPlan INIT_PLAN = {{{ENEMY_ACTION_SPAWN, 2}}, 0, 1};

static const uint8_t PIXIE_HP = MAX_ENEMY_HEALTH[PIXIE];
static const Enemy PIXIE_3_0 = {true, PIXIE, {3, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};
static const Enemy PIXIE_3_1 = {true, PIXIE, {3, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};
static const Enemy PIXIE_3_2 = {true, PIXIE, {3, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};
static const Enemy PIXIE_4_0 = {true, PIXIE, {4, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};
static const Enemy PIXIE_4_1 = {true, PIXIE, {4, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};
static const Enemy PIXIE_4_2 = {true, PIXIE, {4, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};
static const Enemy PIXIE_5_0 = {true, PIXIE, {5, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};
static const Enemy PIXIE_5_1 = {true, PIXIE, {5, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};
static const Enemy PIXIE_5_2 = {true, PIXIE, {5, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, PIXIE_HP, PIXIE_HP, {.none = {}}};

static const uint8_t WISP_HP = MAX_ENEMY_HEALTH[WISP];
static const Enemy WISP_3_0 = {true, WISP, {3, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};
static const Enemy WISP_3_1 = {true, WISP, {3, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};
static const Enemy WISP_3_2 = {true, WISP, {3, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};
static const Enemy WISP_4_0 = {true, WISP, {4, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};
static const Enemy WISP_4_1 = {true, WISP, {4, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};
static const Enemy WISP_4_2 = {true, WISP, {4, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};
static const Enemy WISP_5_0 = {true, WISP, {5, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};
static const Enemy WISP_5_1 = {true, WISP, {5, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};
static const Enemy WISP_5_2 = {true, WISP, {5, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, WISP_HP, WISP_HP, {.none = {}}};

static const uint8_t SCARAB_HP = MAX_ENEMY_HEALTH[SCARAB];
static const Enemy SCARAB_3_0 = {true, SCARAB, {3, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};
static const Enemy SCARAB_3_1 = {true, SCARAB, {3, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};
static const Enemy SCARAB_3_2 = {true, SCARAB, {3, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};
static const Enemy SCARAB_4_0 = {true, SCARAB, {4, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};
static const Enemy SCARAB_4_1 = {true, SCARAB, {4, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};
static const Enemy SCARAB_4_2 = {true, SCARAB, {4, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};
static const Enemy SCARAB_5_0 = {true, SCARAB, {5, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};
static const Enemy SCARAB_5_1 = {true, SCARAB, {5, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};
static const Enemy SCARAB_5_2 = {true, SCARAB, {5, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, SCARAB_HP, SCARAB_HP, {.scarab = {0}}};

static const uint8_t FLUFFY_HP = MAX_ENEMY_HEALTH[FLUFFY];
static const Enemy FLUFFY_3_0 = {true, FLUFFY, {3, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {3, 0}}}};
static const Enemy FLUFFY_3_1 = {true, FLUFFY, {3, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {3, 1}}}};
static const Enemy FLUFFY_3_2 = {true, FLUFFY, {3, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {3, 2}}}};
static const Enemy FLUFFY_4_0 = {true, FLUFFY, {4, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {4, 0}}}};
static const Enemy FLUFFY_4_1 = {true, FLUFFY, {4, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {4, 1}}}};
static const Enemy FLUFFY_4_2 = {true, FLUFFY, {4, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {4, 2}}}};
static const Enemy FLUFFY_5_0 = {true, FLUFFY, {5, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {5, 0}}}};
static const Enemy FLUFFY_5_1 = {true, FLUFFY, {5, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {5, 1}}}};
static const Enemy FLUFFY_5_2 = {true, FLUFFY, {5, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, FLUFFY_HP, FLUFFY_HP, {.fluffy = {true, {5, 2}}}};

static const uint8_t MOLE_HP = MAX_ENEMY_HEALTH[MOLE];
static const Enemy MOLE_3_0 = {true, MOLE, {3, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};
static const Enemy MOLE_3_1 = {true, MOLE, {3, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};
static const Enemy MOLE_3_2 = {true, MOLE, {3, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};
static const Enemy MOLE_4_0 = {true, MOLE, {4, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};
static const Enemy MOLE_4_1 = {true, MOLE, {4, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};
static const Enemy MOLE_4_2 = {true, MOLE, {4, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};
static const Enemy MOLE_5_0 = {true, MOLE, {5, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};
static const Enemy MOLE_5_1 = {true, MOLE, {5, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};
static const Enemy MOLE_5_2 = {true, MOLE, {5, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, MOLE_HP, MOLE_HP, {.mole = {false}}};

static const uint8_t VENUS_HP = MAX_ENEMY_HEALTH[VENUS];
static const Enemy VENUS_3_0 = {true, VENUS, {3, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};
static const Enemy VENUS_3_1 = {true, VENUS, {3, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};
static const Enemy VENUS_3_2 = {true, VENUS, {3, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};
static const Enemy VENUS_4_0 = {true, VENUS, {4, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};
static const Enemy VENUS_4_1 = {true, VENUS, {4, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};
static const Enemy VENUS_4_2 = {true, VENUS, {4, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};
static const Enemy VENUS_5_0 = {true, VENUS, {5, 0}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};
static const Enemy VENUS_5_1 = {true, VENUS, {5, 1}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};
static const Enemy VENUS_5_2 = {true, VENUS, {5, 2}, 0.0f, 2.0f, INIT_PLAN, 0, 0, 0, VENUS_HP, VENUS_HP, {.none = {}}};

static const Enemy WAVES[N_DIFFICULTY_LEVELS][N_WAVES_PER_DIFFICULTY][MAX_ENEMIES] = {
    {
        {PIXIE_5_1},
        {PIXIE_4_1},
        {WISP_4_1},
        {SCARAB_4_1},
        {FLUFFY_5_0},
        {FLUFFY_4_0},
        {MOLE_4_1},
        {VENUS_5_0, VENUS_5_2},
        {PIXIE_4_0, VENUS_5_2},
        {VENUS_5_0, FLUFFY_4_2},
    },
    {
        {PIXIE_3_1},
        {PIXIE_5_0, PIXIE_4_1},
        {PIXIE_4_1, PIXIE_3_2},
        {FLUFFY_3_0},
        {FLUFFY_5_0, FLUFFY_4_2},
        {SCARAB_5_0, SCARAB_5_2},
        {WISP_4_1, SCARAB_5_2},
        {MOLE_5_2, MOLE_3_0},
        {VENUS_3_0, VENUS_3_2},
        {VENUS_4_0, VENUS_4_2},
    },
    {
        {PIXIE_5_2, WISP_4_1},
        {PIXIE_5_0, SCARAB_4_1},
        {WISP_4_1, FLUFFY_5_2},
        {FLUFFY_5_0, SCARAB_4_2},
        {MOLE_4_0, FLUFFY_5_1},
        {MOLE_5_0, SCARAB_4_1},
        {MOLE_3_2, WISP_4_1},
        {MOLE_4_1, PIXIE_5_2},
        {WISP_4_1, VENUS_5_1},
        {MOLE_5_2, VENUS_4_1},
    },
    {
        {PIXIE_5_1, FLUFFY_4_0},
        {MOLE_5_0, MOLE_4_1, MOLE_3_0},
        {PIXIE_5_0, PIXIE_4_1, PIXIE_3_2},
        {MOLE_5_0, MOLE_5_2, SCARAB_4_1},
        {MOLE_5_0, MOLE_5_2, WISP_4_1},
        {PIXIE_4_1, VENUS_5_0, VENUS_5_2},
        {VENUS_5_0, VENUS_5_2, FLUFFY_4_1},
        {MOLE_4_1, VENUS_5_0, VENUS_5_2},
        {VENUS_5_0, VENUS_5_1, VENUS_5_2},
        {VENUS_5_0, VENUS_5_2, VENUS_4_0, VENUS_4_2},
    },
    {
        {FLUFFY_5_0, FLUFFY_4_1, FLUFFY_3_2},
        {PIXIE_3_0, PIXIE_4_2, SCARAB_5_2, SCARAB_4_1},
        {MOLE_3_0, MOLE_3_2, SCARAB_5_2, SCARAB_4_1},
        {SCARAB_5_0, SCARAB_5_2, SCARAB_4_1},
        {PIXIE_4_1, PIXIE_3_0, VENUS_5_0, VENUS_5_2},
        {WISP_4_1, VENUS_5_0, VENUS_5_2},
        {PIXIE_3_2, VENUS_4_1, SCARAB_5_2},
        {WISP_5_2, VENUS_4_2, FLUFFY_5_0},
        {MOLE_4_1, MOLE_5_1, VENUS_5_0},
        {VENUS_5_0, VENUS_5_2, VENUS_4_0, VENUS_4_2, VENUS_3_0, VENUS_3_2},
    },
};

#endif
