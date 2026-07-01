#include "settings.h"

#include <raylib.h>

#include "Sprite.h"
#include "Player.h"

void game_init(void);
void game_loop(void);
void game_close(void);

void _update_game(float dt);
void _draw_game(void);

static Player player = {0};

void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);
    // is_game_running = true;

    init_player(&player);

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
    destroy_player(&player);

    CloseAudioDevice();
    CloseWindow();

    return;
}

void _update_game(float dt) {
    update_player(&player, dt);

    return;
}
void _draw_game(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw_sprite(&player.spr, player.spr.tint);

    EndDrawing();

    return;
}
