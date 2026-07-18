#include "death.h"

void load_death_assets(DeathAssets *a) {
    a->background = LoadTexture("resources/backgrounds/death.png");
}

void unload_death_assets(DeathAssets *a) {
	UnloadTexture(a->background);
}

void load_death_screen(DeathState *s) {}

void unload_death_screen(DeathState *s) {}

void update_death(DeathState *s, Screen *next_screen) {
	if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
        *next_screen = SCREEN_GAMEPLAY;
    }
}

static void draw_death_text() {
    const char *text = "DEATH";
    int width = MeasureText(text, 20);
    DrawText(text, 90 - width / 2, 50, 20, WHITE);
}

static void draw_score(uint16_t score) {
    const char *text = TextFormat("Your Score: %d", score);
    int width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 80, 10, WHITE);
}

static void draw_restart_text() {
    const char *text = "Press Enter to Restart";
    int width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 100, 10, WHITE);
}

void draw_death(const DeathState *s, const DeathAssets *a) {
	DrawTexture(a->background, 0, 0, WHITE);
    draw_death_text();
    draw_score(s->score);
    draw_restart_text();
}
