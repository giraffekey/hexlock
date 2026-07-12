#ifndef UPDATE_BULLET_H
#define UPDATE_BULLET_H

#include "raylib.h"

#include "../player.h"
#include "../enemy.h"
#include "../bullet.h"
#include "../collide.h"
#include "../sounds.h"

void update_bullets(Bullet bullets[], Grid grid, Player *player, Enemy enemies[], const Sounds *sounds);

void update_bullets_time(Bullet bullets[], float dt);

#endif
