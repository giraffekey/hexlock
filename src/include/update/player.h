#ifndef UPDATE_PLAYER_H
#define UPDATE_PLAYER_H

#include "raylib.h"

#include "../player.h"
#include "../enemy.h"
#include "../bullet.h"
#include "../collide.h"
#include "../sounds.h"

/**
 * Updates player action in response to keyboard, gamepad, mouse, and touch input
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 */
void handle_input(Player *player, const Grid grid);

/**
 * Updates player every tick
 * 
 * @param player The current state of the player
 * @param grid The current state of the arena's tiles
 * @param glyphs The current state of the arena's glyphs
 * @param enemies The current state of the arena's enemies
 * @param bullets The current state of the arena's bullets
 * @param sounds The available sound effects to be played
 */
void update_player(Player *player, Grid grid, Glyph glyphs[], Enemy enemies[], Bullet bullets[], const Sounds *sounds);

/**
 * Updates the ID of the enemy whose health is currently being displayed
 * 
 * @param target_enemy The enemy ID to be changed
 * @param player The current state of the player
 * @param enemies The current state of the arena's enemies
 */
void update_target_enemy(size_t *target_enemy, const Player *player, const Enemy enemies[]);

/**
 * Updates the player's time value each frame, used for lerp and animations
 * 
 * @param player The current state of the player
 * @param dt The delta time since last frame
 */
void update_player_time(Player *player, float dt);

#endif
