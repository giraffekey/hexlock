#ifndef UPDATE_PLAYER_H
#define UPDATE_PLAYER_H

#include "raylib.h"

#include "../player.h"
#include "../enemy.h"
#include "../bullet.h"
#include "../collide.h"
#include "../sounds.h"

void handle_input(Player *player, const Grid grid);

void update_player(Player *player, Grid grid, Glyph glyphs[], Enemy enemies[], Bullet bullets[], const Sounds *sounds);

void update_target_enemy(size_t *target_enemy, const Player *player, const Enemy enemies[]);

void update_player_time(Player *player, float dt);

#endif
