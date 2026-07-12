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

void draw_death(const DeathState *s, const DeathAssets *a) {
	DrawTexture(a->background, 0, 0, WHITE);

	char *text;
	int width;

    text = "DEATH";
    width = MeasureText(text, 20);
    DrawText(text, 90 - width / 2, 60, 20, WHITE);

    text = "Press Enter to Restart";
    width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 100, 10, WHITE);
}
