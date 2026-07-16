#include "title.h"

void load_title_assets(TitleAssets *a) {
    a->background = LoadTexture("resources/backgrounds/title.png");
    a->music = LoadMusicStream("resources/music/title.ogg");
}

void unload_title_assets(TitleAssets *a) {
	UnloadTexture(a->background);
    UnloadMusicStream(a->music);
}

void load_title_screen(TitleState *s, const TitleAssets *a) {
    s->is_increasing = true;

    PlayMusicStream(a->music);
}

void unload_title_screen(TitleState *s, const TitleAssets *a) {
    StopMusicStream(a->music);
}

void update_title(TitleState *s, Screen *next_screen) {
    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT)) {
        *next_screen = SCREEN_GAMEPLAY;
    }

    float dt = GetFrameTime();
    if (s->is_increasing) {
        s->t += dt * 1.0f;
        if (s->t >= 1.0f) {
            s->t = 1.0f;
            s->is_increasing = false;
        }
    } else {
        s->t -= dt * 1.0f;
        if (s->t <= 0.0f) {
            s->t = 0.0f;
            s->is_increasing = true;
        }
    }
}

void update_title_music(const TitleAssets *a) {
    UpdateMusicStream(a->music);
}

void draw_title(const TitleState *s, const TitleAssets *a) {
	DrawTexture(a->background, 0, 0, WHITE);

	char *text;
	int width;

    text = "HEXLOCK";
    width = MeasureText(text, 20);
    DrawText(text, 90 - width / 2, 60, 20, WHITE);

    text = "Press Enter to Start";
    width = MeasureText(text, 10);
    DrawText(text, 90 - width / 2, 100, 10, Fade(WHITE, s->t));
}
