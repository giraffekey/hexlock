#ifndef UPDATE_ENEMY_H
#define UPDATE_ENEMY_H

#include "raylib.h"

#include "../player.h"
#include "../enemy.h"
#include "../bullet.h"
#include "../collide.h"
#include "../sounds.h"

void update_enemies(Enemy enemies[], const Grid grid, Player *player, Bullet bullets[], const Sounds *sounds);

void update_enemies_time(Enemy enemies[], float dt);

#endif
