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

static cute_tiled_map_t *map;
static Player *player = NULL;
static CollisionRecs *recs_list = NULL;
static bool is_game_running = true;

void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);
    is_game_running = true;

    map = init_tilemap();

    player = init_player();

    // Initiating test collision rectangles (Delete latter)
    Rectangle collision_test_rec_1 = (Rectangle) { .x= 0, .y= 0, .width=200, .height=200 };

    Rectangle collision_test_rec_2 = (Rectangle) { .x=740, .y=148, .width=200, .height=200 };

    Rectangle collision_test_rec_3 = (Rectangle) { .x=640, .y=572, .width=250, .height=150 };

    Rectangle recs[3] = {
        collision_test_rec_1,
        collision_test_rec_2,
        collision_test_rec_3,
    };
    recs_list = create_collision_recs_list(recs, 3);

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

    draw_player(player);

    for (int i=0; i<recs_list->num; i++) {
        DrawRectangleRec(recs_list->recs[i], RED);
    }

    EndDrawing();

    return;
}
