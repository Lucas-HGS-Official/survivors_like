#include "game.h"

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Bullet.h"
#include "settings.h"

#include "Sprite.h"
#include "Player.h"
#include "CollisionBoxes.h"
#include "TileMap.h"
#include "Gun.h"


void game_init(void);
void game_loop(void);
void game_close(void);

void _update_game(float dt);
void _draw_game(void);


static Tilemap *map = NULL;
static Player *player = NULL;
static float fire_timer = 0;
static Gun *gun = NULL;
static Bullet *bullet = NULL;
static Bullet bullet_list[BULLET_LIST_SIZE] = {0};
static CollisionRecs *recs_list = NULL;
static bool is_game_running = true;
static Camera2D *camera = NULL;
static Sprite *foreground_sprites = NULL;


void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);
    is_game_running = true;

    map = init_tilemap();
    recs_list = map->collision_rec_list;

    player = init_player(map->player_initial_pos);
    camera = init_player_camera(player);
    gun = init_gun(player);
    bullet = init_bullet();
    bullet_list[0] = instance_bullet(bullet, gun->tip, gun->direction);

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

    destroy_bullet(bullet);
    destroy_collision_recs_list(recs_list);
    destroy_player(player);

    destroy_tilemap(map);

    CloseAudioDevice();
    CloseWindow();

    return;
}

void _update_game(float dt) {
    if (WindowShouldClose()) { is_game_running = false; }

    update_player(player, recs_list, dt);
    update_player_camera(camera, player);
    update_gun(gun, player);

    for (int i=0; i<map->obj_blocks_size; i++) {
        foreground_sprites[i] = map->obj_blocks[i].spr;
    }

    Sprite *current_player_sprite = &player->spr[player->facing_direction][player->current_frame];
    foreground_sprites[map->obj_blocks_size] = *current_player_sprite;

    if (IsKeyPressed(KEY_SPACE)) {
        for (int i=0; i<BULLET_LIST_SIZE; i++) {
            if(!bullet_list[i].is_visible && fire_timer <= 0) {
                bullet_list[i] = instance_bullet(bullet, gun->tip, gun->direction);
                fire_timer = .5f;
                break;
            }
        }
    }
    fire_timer -= dt;
    update_bullet_list(bullet_list, BULLET_LIST_SIZE, dt);

    return;
}
void _draw_game(void) {
    BeginDrawing();
    BeginMode2D(*camera);
    ClearBackground(RAYWHITE);

    draw_tilemap(map);
    sort_and_draw_sprite_list(foreground_sprites, map->obj_blocks_size+1);
    draw_bullet_list(bullet_list, BULLET_LIST_SIZE);
    draw_gun(gun);

    EndMode2D();
    EndDrawing();

    return;
}
