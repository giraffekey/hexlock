#ifndef COLLIDE_H
#define COLLIDE_H

#include "raylib.h"

#include "glyph.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "sounds.h"

void play_damage_sound(DamageStatus status, const Sounds *sounds);

void check_player_glyph_collision(Player *player, Glyph glyphs[], uint16_t *score, const Sounds *sounds);

void on_player_enemy_collision(Player *player, Enemy *enemy, bool is_enemy_update, Bullet bullets[], const Sounds *sounds);

void check_player_enemy_collision(Player *player, Enemy enemies[], Bullet bullets[], const Sounds *sounds);

void check_enemy_player_collision(Enemy *enemy, Player *player, Bullet bullets[], const Sounds *sounds);

void check_bullet_player_collision(Bullet *bullet, Player *player, Bullet bullets[], const Sounds *sounds);

void check_player_bullet_collision(Player *player, Bullet bullets[], const Sounds *sounds);

void check_bullet_enemy_collision(Bullet *bullet, Enemy enemies[], Bullet bullets[], const Sounds *sounds);

void check_enemy_bullet_collision(Enemy *enemy, Bullet bullets[], const Sounds *sounds);

#endif
