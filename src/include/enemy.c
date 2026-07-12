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
