#include "win.h"

void load_win_assets(WinAssets *a) {
    a->background = LoadTexture("resources/backgrounds/win.png");
}

void unload_win_assets(WinAssets *a) {
	UnloadTexture(a->background);
}

void load_win_screen(WinState *s) {}

void update_win(WinState *s, Screen *next_screen) {
	if (IsKeyPressed(KEY_ENTER)) *next_screen = SCREEN_GAMEPLAY;
}

void draw_win(const WinState *s, const WinAssets *a) {
	DrawTexture(a->background, 0, 0, WHITE);

	char *text;
	int width;

    text = "WIN";
    width = MeasureText(text, 16);
    DrawText(text, 90 - width / 2, 60, 16, WHITE);

    text = "Press Enter to Play Again";
    width = MeasureText(text, 8);
    DrawText(text, 90 - width / 2, 100, 8, WHITE);
}
