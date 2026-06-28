#include "settings.h"
#include <raylib.h>

void game_init(void);
void game_loop(void);
void game_close(void);

void _update_game(float dt);
void _draw_game(void);


void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);
    // is_game_running = true;

    return;
}
void game_loop(void) {
    while (!WindowShouldClose()) {
        _update_game(GetFrameTime());
        _draw_game();
    }

    return;
}
void game_close(void) {
    CloseAudioDevice();
    CloseWindow();

    return;
}

void _update_game(float dt) {

    return;
}
void _draw_game(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    EndDrawing();

    return;
}
