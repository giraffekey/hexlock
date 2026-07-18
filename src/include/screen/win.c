#include "win.h"

void load_win_assets(WinAssets *a) {
    a->background = LoadTexture("resources/backgrounds/win.png");
}

void unload_win_assets(WinAssets *a) {
	UnloadTexture(a->background);
}

void load_win_screen(WinState *s) {
    StorageData data = load_data();
    if (s->score > data.high_score) {
        data.high_score = s->score;
    }
    s->high_score = data.high_score;
    save_data(&data);
}

void unload_win_screen(WinState *s) {}

void update_win(WinState *s, Screen *next_screen) {
    bool is_key_pressed = IsKeyPressed(KEY_ENTER);
    bool is_gamepad_pressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT);
    bool is_clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool is_tapped = GetGestureDetected() == GESTURE_TAP;
    if (is_key_pressed || is_gamepad_pressed || is_clicked || is_tapped) {
        *next_screen = SCREEN_GAMEPLAY;
    }
}

static void draw_win_text() {
    const char *text = "WIN";
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
    const char *text = "Press Enter to Play Again";
    int width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 120, 10, WHITE);
}

void draw_win(const WinState *s, const WinAssets *a) {
	DrawTexture(a->background, 0, 0, WHITE);
    draw_win_text();
    draw_score(s->score);
    draw_high_score(s->high_score);
    draw_restart_text();
}
