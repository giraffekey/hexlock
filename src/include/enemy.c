#include "enemy.h"

void add_step(EnemyPlan *plan, EnemyActionU8 action, uint8_t cooldown) {
    plan->steps[plan->size++] = (EnemyStep){action, cooldown};
}

EnemyStep current_step(const Enemy *enemy) {
    return enemy->plan.steps[enemy->plan.index];
}

EnemyAction current_action(const Enemy *enemy) {
    return current_step(enemy).action;
}

bool is_enemy_dead(const Enemy *enemy) {
    return current_action(enemy) == ENEMY_ACTION_DEATH && enemy->cooldown == 0;
}

bool can_be_hit(const Enemy *enemy) {
    EnemyAction action = current_action(enemy);
    switch (enemy->type) {
    case WISP: return action != ENEMY_ACTION_TELEPORT && action != ENEMY_ACTION_TELEPORT_TO_PLAYER;
    case SCARAB: return action != ENEMY_ACTION_SPIN;
    case FLUFFY: return action != ENEMY_ACTION_RETURN;
    case MOLE: return !enemy->data.mole.hidden;
    default: return true;
    }
    return true;
}

int current_enemy_frame(const Enemy *enemy) {
    switch (enemy->type) {
    case PIXIE:
        if (enemy->hit) return 6 + enemy->hit % 2;
        switch (current_action(enemy)) {
        case ENEMY_ACTION_WAIT:
        case ENEMY_ACTION_UP:
        case ENEMY_ACTION_DOWN: return (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_POWDER: return 2 + min((int)(enemy->t * enemy->t_rate), 1);
        case ENEMY_ACTION_SPAWN: return 4 + min((int)(enemy->t * enemy->t_rate), 1);
        case ENEMY_ACTION_DEATH: return 8 + min((int)(enemy->t * enemy->t_rate), 1);
        }
        break;
    case WISP:
        if (enemy->hit) return 8 + enemy->hit % 2;
        switch (current_action(enemy)) {
        case ENEMY_ACTION_WAIT: return (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_TELEPORT:
        case ENEMY_ACTION_TELEPORT_TO_PLAYER: return 2 + min((int)(enemy->t * enemy->t_rate), 1);
        case ENEMY_ACTION_MELEE_ATTACK: return 4 + min((int)(enemy->t * enemy->t_rate), 1);
        case ENEMY_ACTION_SPAWN: return 6 + min((int)(enemy->t * enemy->t_rate), 1);
        case ENEMY_ACTION_DEATH: return 10 + min((int)(enemy->t * enemy->t_rate), 1);
        }
        break;
    case SCARAB:
        if (enemy->hit) return 6 + (int)(enemy->t * enemy->t_rate) % 2;
        switch (current_action(enemy)) {
        case ENEMY_ACTION_LEFT:
        case ENEMY_ACTION_RIGHT:
        case ENEMY_ACTION_UP:
        case ENEMY_ACTION_DOWN: return (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_SPIN: return 2 + (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_SPAWN: return 4 + (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_DEATH: return 8 + (int)(enemy->t * enemy->t_rate) % 2;
        }
        break;
    case FLUFFY:
        if (enemy->hit) return 9 + (int)(enemy->t * enemy->t_rate) % 2;
        switch (current_action(enemy)) {
        case ENEMY_ACTION_WAIT:
        case ENEMY_ACTION_REVERSE: return (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_UP:
        case ENEMY_ACTION_DOWN: return 2 + min((int)(enemy->t * enemy->t_rate), 1);
        case ENEMY_ACTION_LEFT: return 4 + (enemy->data.fluffy.origin.x - enemy->pos.x) % 2;
        case ENEMY_ACTION_RETURN: return 6;
        case ENEMY_ACTION_SPAWN: return 7 + (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_DEATH: return 11 + (int)(enemy->t * enemy->t_rate) % 2;
        }
        break;
    case MOLE:
        if (enemy->hit) return 8 + (int)(enemy->t * enemy->t_rate) % 2;
        switch (current_action(enemy)) {
        case ENEMY_ACTION_WAIT: return (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_HIDE:
        case ENEMY_ACTION_REVEAL: return 2;
        case ENEMY_ACTION_PELLET: return 3 + (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_LEFT:
        case ENEMY_ACTION_RIGHT:
        case ENEMY_ACTION_UP:
        case ENEMY_ACTION_DOWN: return 5;
        case ENEMY_ACTION_SPAWN: return 6 + (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_DEATH: return 10 + (int)(enemy->t * enemy->t_rate) % 2;
        }
        break;
    case VENUS:
        if (enemy->hit) return 5 + (int)(enemy->t * enemy->t_rate) % 2;
        switch (current_action(enemy)) {
        case ENEMY_ACTION_WAIT: return 0;
        case ENEMY_ACTION_SPINY: return 1 + (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_SPAWN: return 3 + (int)(enemy->t * enemy->t_rate) % 2;
        case ENEMY_ACTION_DEATH: return 7 + (int)(enemy->t * enemy->t_rate) % 2;
        }
        break;
    }
    return 0;
}

void hit_enemy(Enemy *enemy) {
    if (current_action(enemy) != ENEMY_ACTION_DEATH) enemy->hit = 2;
}

void kill_enemy(Enemy *enemy) {
    EnemyPlan plan = {0};
    add_step(&plan, ENEMY_ACTION_DEATH, 2);
    enemy->plan = plan;
    enemy->cooldown = 2;
    enemy->t = 0.0f;
}
