#include "death.h"

void load_death_assets(DeathAssets *a) {
    a->background = LoadTexture("resources/backgrounds/death.png");
}

void unload_death_assets(DeathAssets *a) {
	UnloadTexture(a->background);
}

void load_death_screen(DeathState *s) {
    StorageData data = load_data();
    if (s->score > data.high_score) {
        data.high_score = s->score;
    }
    s->high_score = data.high_score;
    save_data(&data);
}

void unload_death_screen(DeathState *s) {}

void update_death(DeathState *s, Screen *next_screen) {
	if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
        *next_screen = SCREEN_GAMEPLAY;
    }
}

static void draw_death_text() {
    const char *text = "DEATH";
    int width = MeasureText(text, 20);
    DrawText(text, 90 - width / 2, 60, 20, WHITE);
}

static void draw_score(uint16_t score) {
    const char *text = TextFormat("Score: %d", score);
    int width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 100, 10, WHITE);
}

static void draw_high_score(uint16_t high_score) {
    const char *text = TextFormat("High Score: %d", high_score);
    int width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 10, 10, WHITE);
}

static void draw_restart_text() {
    const char *text = "Press Enter to Restart";
    int width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 120, 10, WHITE);
}

void draw_death(const DeathState *s, const DeathAssets *a) {
	DrawTexture(a->background, 0, 0, WHITE);
    draw_death_text();
    draw_score(s->score);
    draw_high_score(s->high_score);
    draw_restart_text();
}
