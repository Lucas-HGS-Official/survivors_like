#include "settings.h"

#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>

// #include "Sprite.h"
#include "Player.h"

void game_init(void);
void game_loop(void);
void game_close(void);

void _update_game(float dt);
void _draw_game(void);

static Player *player = NULL;
static bool is_game_running = true;

void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);
    is_game_running = true;

    player = init_player();

    return;
}
void game_loop(void) {
    while (is_game_running) {
        _update_game(GetFrameTime());
        _draw_game();
    }

    return;
}
void game_close(void) {
    destroy_player(player);

    CloseAudioDevice();
    CloseWindow();

    return;
}

void _update_game(float dt) {
    update_player(player, dt);

    if (WindowShouldClose()) { is_game_running = false; }

    return;
}
void _draw_game(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // draw_sprite(player.spr, player.spr.tint);
    draw_player(player);

    EndDrawing();

    return;
}
