#include "game.h"

#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>

#include "Sprite.h"
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
static Camera2D *camera = NULL;
static Sprite *foreground_sprites;

void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);
    is_game_running = true;

    map = init_tilemap();
    recs_list = map->collision_rec_list;

    player = init_player(map->player_initial_pos);
    camera = init_player_camera(player);

    // Sprite foreground_sprites[map->obj_blocks_size+1];
    foreground_sprites = (Sprite*)MemAlloc(sizeof(Sprite) * (map->obj_blocks_size+1));
    for (int i=0; i<map->obj_blocks_size; i++) {
        foreground_sprites[i] = map->obj_blocks[i].spr;
    }

    Sprite *player_sprite = &player->spr[0][0];
    foreground_sprites[map->obj_blocks_size] = *player_sprite;

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
    MemFree(camera);
    MemFree(foreground_sprites);

    destroy_collision_recs_list(recs_list);
    destroy_player(player);

    destroy_tilemap(map);

    CloseAudioDevice();
    CloseWindow();

    return;
}

void _update_game(float dt) {
    update_player(player, recs_list, dt);
    update_player_camera(camera, player);

    for (int i=0; i<map->obj_blocks_size; i++) {
        foreground_sprites[i] = map->obj_blocks[i].spr;
    }

    Sprite *current_player_sprite = &player->spr[player->facing_direction][player->current_frame];
    foreground_sprites[map->obj_blocks_size] = *current_player_sprite;

    if (WindowShouldClose()) { is_game_running = false; }

    return;
}
void _draw_game(void) {
    BeginDrawing();
    BeginMode2D(*camera);
    ClearBackground(RAYWHITE);

    draw_tilemap(map);
    sort_and_draw_sprite_list(foreground_sprites, map->obj_blocks_size+1);

    EndMode2D();
    EndDrawing();

    return;
}
