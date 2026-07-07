#include "game.h"

#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>

#include "settings.h"

#include "Player.h"
#include "CollisionBoxes.h"
#include "TileMap.h"


void game_init(void);
void game_loop(void);
void game_close(void);

void _update_game(float dt);
void _draw_game(void);

static Tilemap *map;
static Player *player = NULL;
static CollisionRecs *recs_list = NULL;
static bool is_game_running = true;

void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);
    is_game_running = true;

    map = init_tilemap();
    recs_list = map->obj_blocks_hitboxes;

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
    destroy_collision_recs_list(recs_list);
    destroy_player(player);

    CloseAudioDevice();
    CloseWindow();

    destroy_tilemap(map);

    return;
}

void _update_game(float dt) {
    update_player(player, recs_list, dt);

    if (WindowShouldClose()) { is_game_running = false; }

    return;
}
void _draw_game(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw_tilemap(map);
    draw_player(player);

    EndDrawing();

    return;
}
