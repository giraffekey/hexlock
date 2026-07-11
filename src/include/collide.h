#ifndef COLLIDE_H
#define COLLIDE_H

#include "glyph.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

void check_player_glyph_collision(Player *player, Glyph glyphs[]);

void on_player_enemy_collision(Player *player, Enemy *enemy, Bullet bullets[]);

void check_player_enemy_collision(Player *player, Enemy enemies[], Bullet bullets[]);

void check_enemy_player_collision(Enemy *enemy, Player *player, Bullet bullets[]);

void check_bullet_player_collision(Bullet *bullet, Player *player, Bullet bullets[]);

void check_player_bullet_collision(Player *player, Bullet bullets[]);

void check_bullet_enemy_collision(Bullet *bullet, Enemy enemies[], Bullet bullets[]);

void check_enemy_bullet_collision(Enemy *enemy, Bullet bullets[]);

#endif
