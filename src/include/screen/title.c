#include "title.h"

void load_title_assets(TitleAssets *a) {
    a->background = LoadTexture("resources/backgrounds/title.png");
}

void unload_title_assets(TitleAssets *a) {
	UnloadTexture(a->background);
}

void load_title_screen(TitleState *s) {}

void update_title(TitleState *s, Screen *next_screen) {
	if (IsKeyPressed(KEY_ENTER)) *next_screen = SCREEN_GAMEPLAY;
}

void draw_title(const TitleState *s, const TitleAssets *a) {
	DrawTexture(a->background, 0, 0, WHITE);

	char *text;
	int width;

    text = "HEXLOCK";
    width = MeasureText(text, 16);
    DrawText(text, 90 - width / 2, 60, 16, WHITE);

    text = "Press Enter to Start";
    width = MeasureText(text, 8);
    DrawText(text, 90 - width / 2, 100, 8, WHITE);
}
