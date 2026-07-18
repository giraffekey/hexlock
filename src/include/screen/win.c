#include "win.h"

void load_win_assets(WinAssets *a) {
    a->background = LoadTexture("resources/backgrounds/win.png");
}

void unload_win_assets(WinAssets *a) {
	UnloadTexture(a->background);
}

void load_win_screen(WinState *s) {}

void unload_win_screen(WinState *s) {}

void update_win(WinState *s, Screen *next_screen) {
    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
        *next_screen = SCREEN_GAMEPLAY;
    }
}

static void draw_win_text() {
    const char *text = "WIN";
    int width = MeasureText(text, 20);
    DrawText(text, 90 - width / 2, 50, 20, WHITE);
}

static void draw_score(uint16_t score) {
    const char *text = TextFormat("Your Score: %d", score);
    int width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 80, 10, WHITE);
}

static void draw_restart_text() {
    const char *text = "Press Enter to Play Again";
    int width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 100, 10, WHITE);
}

void draw_win(const WinState *s, const WinAssets *a) {
	DrawTexture(a->background, 0, 0, WHITE);
    draw_win_text();
    draw_score(s->score);
    draw_restart_text();
}
