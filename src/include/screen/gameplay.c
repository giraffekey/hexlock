#include "gameplay.h"

static void spawn_wave(Enemy enemies[], const Grid grid, uint8_t difficulty) {
    memcpy(enemies, WAVES[difficulty][rand() % N_WAVES_PER_DIFFICULTY], sizeof(Enemy) * MAX_ENEMIES);

    bool is_used[18] = {0};
    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
    	Enemy *enemy = &enemies[i];
    	if (enemy->exists) {
            if (is_player_tile(grid, enemy->pos)) {
                if (enemy->pos.x == N_COLS - 1) {
                    enemy->exists = false;
                } else {
                    Position behind = {enemy->pos.x + 1, enemy->pos.y};
                    if (!is_player_tile(grid, behind) && !is_used[behind.y * 6 + behind.x]) {
                        enemy->pos = behind;
                    } else {
                        enemy->exists = false;
                    }
                }
            }

            if (enemy->exists) {
                uint8_t cooldown = current_step(enemy).cooldown;
                enemy->cooldown = cooldown;
                enemy->t_rate = (float)cooldown;

                switch (enemy->type) {
                case FLUFFY:
                    enemy->data.fluffy.origin = enemy->pos;
                    break;
                case SCARAB:
                    enemy->data.scarab.orient = rand() % 4;
                    break;
                }

                is_used[enemy->pos.y * 6 + enemy->pos.x] = true;
            }
	    }
    }
}

static void update_countdown(GameplayState *s) {
	s->countdown--;
	if (s->countdown == 0) {
		uint8_t difficulty = s->wave / 4;
		spawn_wave(s->enemies, s->grid, difficulty);
		s->is_clear = false;
	}
}

static bool update_wave(GameplayState *s) {
    bool is_only_player = true;
    for (size_t y = 0; y < N_ROWS; ++y) {
        for (size_t x = 0; x < N_COLS; ++x) {
            if (!s->grid[y][x].is_player) {
                is_only_player = false;
                break;
            }
        }
    }

    if (is_only_player) return true;

	bool is_clear = true;
	for (size_t i = 0; i < MAX_ENEMIES; ++i) {
		const Enemy *enemy = &s->enemies[i];
		if (enemy->exists) {
			is_clear = false;
			break;
		}
	}

	if (is_clear) {
        s->wave++;
		if (s->wave == N_WAVES) {
            return true;
		} else {
			s->countdown = 30;
			s->is_clear = true;
		}
	}
    return false;
}

void load_gameplay_assets(GameplayAssets *a) {
    a->background = LoadTexture("resources/backgrounds/gameplay.png");
    a->tiles_sprite = LoadTexture("resources/sprites/tiles.png");
    a->player_sprite = LoadTexture("resources/sprites/player.png");
    a->signs_sprite = LoadTexture("resources/sprites/signs.png");
    a->enemies_sprite = LoadTexture("resources/sprites/enemies.png");
    a->bullets_sprite = LoadTexture("resources/sprites/bullets.png");
    a->status_sprite = LoadTexture("resources/sprites/status.png");
    a->music = LoadMusicStream("resources/music/gameplay.wav");
}

void unload_gameplay_assets(GameplayAssets *a) {
    UnloadTexture(a->background);
    UnloadTexture(a->tiles_sprite);
    UnloadTexture(a->player_sprite);
    UnloadTexture(a->signs_sprite);
    UnloadTexture(a->enemies_sprite);
    UnloadTexture(a->bullets_sprite);
    UnloadTexture(a->status_sprite);
    UnloadMusicStream(a->music);
}

void load_gameplay_screen(GameplayState *s, const GameplayAssets *a) {
	s->tick = TICK_RATE;

    GlyphSpawner spawner = {{}, GLYPH_SPAWN_RATE, 0};
    generate_glyph_chunk(&spawner);
    s->glyph_spawner = spawner;

    for (size_t y = 0; y < N_ROWS; ++y) {
        for (size_t x = 0; x < N_COLS / 2; ++x) {
            s->grid[y][x].is_player = true;
        }
        for (size_t x = N_COLS / 2; x < N_COLS; ++x) {
            s->grid[y][x].is_player = false;
        }
    }

    s->player.pos = (Position){1, 1};
    s->player.t_rate = 2.0f;
    s->player.next_t_rate = 2.0f;
    s->player.hp = MAX_PLAYER_HEALTH;
    
    s->countdown = 30;
    s->is_clear = true;

    PlayMusicStream(a->music);
}

void unload_gameplay_screen(GameplayState *s, const GameplayAssets *a) {
    StopMusicStream(a->music);
}

static void update_time(GameplayState *s, float dt) {
    update_glyphs_time(s->glyphs, dt);
    update_player_time(&s->player, dt);
    update_enemies_time(s->enemies, dt);
    update_bullets_time(s->bullets, dt);
}

static void update_tick(GameplayState *s, const Sounds *sounds, Screen *next_screen) {
    update_glyphs(s->glyphs);
    update_player(&s->player, s->grid, s->glyphs, s->enemies, s->bullets, sounds);
    update_enemies(s->enemies, s->grid, &s->player, s->bullets, sounds);
    update_bullets(s->bullets, s->grid, &s->player, s->enemies, sounds);
    update_glyph_spawner(&s->glyph_spawner, s->glyphs, s->wave, s->grid, &s->player);
    update_target_enemy(&s->target_enemy, &s->player, s->enemies);

    if (s->player.hp == 0) {
        *next_screen = SCREEN_DEATH;
        PlaySound(sounds->death);
    } else if (s->is_clear) {
        update_countdown(s);
    } else {
        bool win = update_wave(s);
        if (win) {
            *next_screen = SCREEN_WIN;
            PlaySound(sounds->win);
        }
    }
}

void update_gameplay(GameplayState *s, const Sounds *sounds, Screen *next_screen) {
    handle_input(&s->player, s->grid);

    float dt = GetFrameTime();
    update_time(s, dt);

    s->tick -= dt;
    if (s->tick <= 0.0f) {
        update_tick(s, sounds, next_screen);
        s->tick += TICK_RATE;
    }
}

void update_gameplay_music(const GameplayAssets *a) {
    UpdateMusicStream(a->music);
}

static Vector2 tile_to_grid_pos(Position pos) {
    return (Vector2){22 + (float)pos.x * 24, 80 + (float)pos.y * 24};
}

static void draw_background(Texture2D background) {
    DrawTexture(background, 0, 0, WHITE);
}

static void draw_grid(const Grid grid, Texture2D sprite) {
    Rectangle src;
    Vector2 pos;
    for (size_t y = 0; y < N_ROWS; ++y) {
        for (size_t x = 0; x < N_COLS; ++x) {
            src = (Rectangle){grid[y][x].type * 16, 0, 16, 16};
            pos = tile_to_grid_pos((Position){x, y});
            DrawTextureRec(sprite, src, pos, WHITE);

            src = (Rectangle){grid[y][x].is_player ? 0 : 32, 16, 32, 32};
            pos = (Vector2){14 + (float)x * 24, 72 + (float)y * 24};
            DrawTextureRec(sprite, src, pos, WHITE);
        }
    }
}

static void draw_player(const Player *player, Texture2D sprite) {
    Vector2 from = tile_to_grid_pos(player->pos);
    Vector2 to;
    Vector2 final;

    switch (player->action) {
    case ACTION_LEFT:
        to.x = from.x - 24;
        to.y = from.y;
        final = Vector2Lerp(from, to, player->t);
        break;
    case ACTION_RIGHT:
        to.x = from.x + 24;
        to.y = from.y;
        final = Vector2Lerp(from, to, player->t);
        break;
    case ACTION_UP:
        to.x = from.x;
        to.y = from.y - 24;
        final = Vector2Lerp(from, to, player->t);
        break;
    case ACTION_DOWN:
        to.x = from.x;
        to.y = from.y + 24;
        final = Vector2Lerp(from, to, player->t);
        break;
    default:
        final = from;
        break;
    }

    int frame;
    switch (player->action) {
    case ACTION_WAIT:
        frame = player->wait_frame / 2;
        break;
    case ACTION_LEFT:
    case ACTION_RIGHT:
    case ACTION_UP:
    case ACTION_DOWN:
        frame = 2 + min((int)(player->t * 2.0f), 1);
        break;
    case ACTION_MISSILE:
    case ACTION_HEX:
        frame = 4 + min((int)(player->t * 2.0f), 1);
        break;
    }

    Rectangle src = {frame * 16, 0, 16, 16};
    DrawTextureRec(sprite, src, final, WHITE);
}

static void draw_enemies(const Enemy enemies[], Texture2D sprite) {
    for (size_t i = 0; i < MAX_ENEMIES; ++i) {
        const Enemy *enemy = &enemies[i];
        if (enemy->exists) {
            Vector2 from = tile_to_grid_pos(enemy->pos);
            Vector2 to;
            Vector2 final;

            switch (current_action(enemy)) {
            case ENEMY_ACTION_LEFT:
                to.x = from.x - 24;
                to.y = from.y;
                final = Vector2Lerp(from, to, enemy->t);
                break;
            case ENEMY_ACTION_RIGHT:
                to.x = from.x + 24;
                to.y = from.y;
                final = Vector2Lerp(from, to, enemy->t);
                break;
            case ENEMY_ACTION_UP:
                to.x = from.x;
                to.y = from.y - 24;
                final = Vector2Lerp(from, to, enemy->t);
                break;
            case ENEMY_ACTION_DOWN:
                to.x = from.x;
                to.y = from.y + 24;
                final = Vector2Lerp(from, to, enemy->t);
                break;
            case ENEMY_ACTION_RETURN:
            	to.x = 22 + (float)enemy->data.fluffy.origin.x * 24;
            	to.y = 80 + (float)enemy->data.fluffy.origin.y * 24;
                final = Vector2Lerp(from, to, enemy->t);
            	break;
            default:
                final = from;
                break;
            }

            int frame;
            switch (enemy->type) {
            case PIXIE:
            	switch (current_action(enemy)) {
            	case ENEMY_ACTION_WAIT:
            	case ENEMY_ACTION_UP:
            	case ENEMY_ACTION_DOWN:
			        frame = (int)(enemy->t * enemy->t_rate) % 2;
            		break;
            	case ENEMY_ACTION_POWDER:
			        frame = 2 + min((int)(enemy->t * enemy->t_rate), 1);
            		break;
            	}
            	break;
            case WISP:
            	switch (current_action(enemy)) {
            	case ENEMY_ACTION_WAIT:
			        frame = (int)(enemy->t * enemy->t_rate) % 2;
            		break;
            	case ENEMY_ACTION_TELEPORT:
            	case ENEMY_ACTION_TELEPORT_TO_PLAYER:
			        frame = 2 + min((int)(enemy->t * enemy->t_rate), 1);
            		break;
            	case ENEMY_ACTION_MELEE_ATTACK:
			        frame = 4 + min((int)(enemy->t * enemy->t_rate), 1);
            		break;
            	}
            	break;
            case SCARAB:
            	switch (current_action(enemy)) {
            	case ENEMY_ACTION_LEFT:
            	case ENEMY_ACTION_RIGHT:
            	case ENEMY_ACTION_UP:
            	case ENEMY_ACTION_DOWN:
			        frame = (int)(enemy->t * enemy->t_rate) % 2;
            		break;
            	case ENEMY_ACTION_SPIN:
			        frame = 2 + (int)(enemy->t * enemy->t_rate) % 2;
            		break;
            	}
            	break;
            case FLUFFY:
            	switch (current_action(enemy)) {
            	case ENEMY_ACTION_WAIT:
                case ENEMY_ACTION_REVERSE:
                    frame = (int)(enemy->t * enemy->t_rate) % 2;
			        break;
            	case ENEMY_ACTION_UP:
            	case ENEMY_ACTION_DOWN:
			        frame = 2 + min((int)(enemy->t * enemy->t_rate), 1);
            		break;
            	case ENEMY_ACTION_LEFT:
                    frame = 4 + (enemy->data.fluffy.origin.x - enemy->pos.x) % 2;
            		break;
            	case ENEMY_ACTION_RETURN:
            		frame = 6;
            		break;
            	}
            	break;
            case MOLE:
            	switch (current_action(enemy)) {
            	case ENEMY_ACTION_WAIT:
			        frame = (int)(enemy->t * enemy->t_rate) % 2;
			        break;
			    case ENEMY_ACTION_HIDE:
			    case ENEMY_ACTION_REVEAL:
			    	frame = 2;
			    	break;
            	case ENEMY_ACTION_PELLET:
			        frame = 3 + (int)(enemy->t * enemy->t_rate) % 2;
            		break;
            	case ENEMY_ACTION_LEFT:
            	case ENEMY_ACTION_RIGHT:
            	case ENEMY_ACTION_UP:
            	case ENEMY_ACTION_DOWN:
            		frame = 5;
            		break;
            	}
            	break;
            case VENUS:
            	switch (current_action(enemy)) {
            	case ENEMY_ACTION_WAIT:
            		frame = 0;
			        break;
            	case ENEMY_ACTION_SPINY:
			        frame = 1 + (int)(enemy->t * enemy->t_rate) % 2;
			        break;
            	}
            	break;
            }

		    Rectangle src = {frame * 16, enemy->type * 16, 16, 16};
            switch (enemy->type) {
            case SCARAB:
            	Rectangle dest = {final.x + 8, final.y + 8, 16, 16};
            	Vector2 origin = {8, 8};
            	float rot = 90 + enemy->data.scarab.orient * 90;
            	DrawTexturePro(sprite, src, dest, origin, rot, WHITE);
            	break;
            default:
			    DrawTextureRec(sprite, src, final, WHITE);
			    break;
            }
        }
    }
}

static void draw_bullets(const Bullet bullets[], Texture2D sprite) {
    for (size_t i = 0; i < MAX_BULLETS; ++i) {
        const Bullet *bullet = &bullets[i];
        if (bullet->exists) {
            Vector2 from = tile_to_grid_pos(bullet->pos);
            
            Vector2 to;
            if (bullet->type == BULLET_TWISTER) {
                const BulletTwisterData *data = &bullet->data.twister;
                to.x = bullet->is_player ? from.x + 24 : from.x - 24;
                to.y = data->is_moving_up ? from.y - 24 : from.y + 24;
            } else {
                to.x = bullet->is_player ? from.x + 24 : from.x - 24;
                to.y = from.y;
            }
            Vector2 final = Vector2Lerp(from, to, bullet->t);
            
            int frame;
            switch (bullet->type) {
            case BULLET_MISSILE:
                frame = 0;
                break;
            case BULLET_FIREBALL:
                frame = 1;
                break;
            case BULLET_WILDFIRE:
                frame = 2;
                break;
            case BULLET_FIRE_WAVE:
                frame = 3;
                break;
            case BULLET_FIRE_BLAST:
                frame = 4;
                break;
            case BULLET_FLAME:
                frame = 5;
                break;
            case BULLET_TWISTER:
                frame = 6 + bullet->data.twister.cooldown % 2;
                break;
            case BULLET_SHOCKWAVE:
                frame = 8;
                break;
            case BULLET_POWDER:
                frame = 9;
                break;
            case BULLET_PELLET:
                frame = 10;
                break;
            case BULLET_SPINY:
                frame = 11;
                break;
            case BULLET_WHIRLPOOL:
            case BULLET_HURRICANE:
                frame = 12; // Placeholder
                break;
            }
            
            float w = bullet->is_player ? 16 : -16;
            Rectangle src = {frame * 16, 0, w, 16};
            Rectangle dest = {final.x + 8, final.y + 8, 16, 16};
            Vector2 origin = {8, 8};
            DrawTexturePro(sprite, src, dest, origin, 0, WHITE);
        }
    }
}

static void draw_glyphs(const Glyph glyphs[], Texture sprite) {
    for (size_t i = 0; i < MAX_GLYPHS; ++i) {
        const Glyph *glyph = &glyphs[i];
        if (glyph->exists) {
            Rectangle src = {glyph->sign * 16, 0, 16, 16};
            Vector2 pos = tile_to_grid_pos(glyph->pos);
            DrawTextureRec(sprite, src, pos, Fade(WHITE, glyph->t));
        }
    }
}

static void draw_signs(const SignU8 signs[], size_t n_signs, Hex hex, Texture sprite) {
    for (size_t i = 0; i < n_signs; ++i) {
        Rectangle src = {signs[i] * 16, 0, 16, 16};
        Vector2 pos = {58 + i * 24, 30};
        DrawTextureRec(sprite, src, pos, WHITE);
    }

    if (hex.valid) {
        const char *text = get_hex_desc(hex);
        int width = MeasureText(text, 8);
        DrawText(text, 90 - width / 2, 56, 8, WHITE);
    }
}

static void draw_player_status(uint8_t hp, Texture2D sprite) {
    for (uint8_t i = 0; i < MAX_PLAYER_HEALTH; ++i) {
        Rectangle src = {i < hp ? 0 : 8, 0, 6, 16};
        Vector2 pos = {22 + i * 6, 164};
        DrawTextureRec(sprite, src, pos, WHITE);
    }
}

static void draw_hex_queue(const Hex hexes[], size_t n_hexes, Texture2D sprite) {
    for (size_t i = 0; i < n_hexes; ++i) {
        Hex hex = hexes[i];
        if (hex.valid) {
            Rectangle src = {hex.sign * 16, hex.omega ? 32 : 16, 16, 16};
            Vector2 pos = {10 + i * 16, 150};
            DrawTextureRec(sprite, src, pos, WHITE);
        }
    }
}

static void draw_enemy_status(uint8_t hp, uint8_t max_hp, Texture2D sprite) {
    for (uint8_t i = 0; i < max_hp; ++i) {
        Rectangle src = {i < hp ? 16 : 24, 0, 6, 16};
        Vector2 pos = {158 - i * 6, 164};
        DrawTextureRec(sprite, src, pos, WHITE);
    }
}

static void draw_wave(uint8_t wave) {
    const char *text = TextFormat("Wave: %d", wave);
    int width = MeasureText(text, 8);
    DrawText(text, 90 - width / 2, 8, 8, WHITE);
}

static void draw_countdown(uint8_t secs) {
    const char *text = TextFormat("%d", secs);
    int width = MeasureText(text, 32);
    DrawText(text, 90 - width / 2, 96, 32, WHITE);
}

void draw_gameplay(const GameplayState *s, const GameplayAssets *a) {
    draw_background(a->background);
    draw_grid(s->grid, a->tiles_sprite);
    draw_glyphs(s->glyphs, a->signs_sprite);
    draw_player(&s->player, a->player_sprite);
    draw_enemies(s->enemies, a->enemies_sprite);
    draw_bullets(s->bullets, a->bullets_sprite);
    draw_signs(s->player.signs, s->player.n_signs, s->player.hexes[0], a->signs_sprite);
    
    draw_player_status(s->player.hp, a->status_sprite);
    draw_hex_queue(s->player.hexes, min(s->player.n_hexes, MAX_HEXES), a->signs_sprite);

    const Enemy *target = &s->enemies[s->target_enemy];
    if (target->exists) draw_enemy_status(target->hp, MAX_ENEMY_HEALTH[target->type], a->status_sprite);

    draw_wave(s->wave + 1);
    if (s->countdown > 0) draw_countdown((s->countdown + 9) / 10);
}
