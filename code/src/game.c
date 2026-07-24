#include "game.h"

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>

#include "settings.h"

#include "Sprite.h"
#include "Player.h"
#include "CollisionBoxes.h"
#include "TileMap.h"
#include "Gun.h"
#include "Bullet.h"
#include "Enemy.h"


void _update_game(float dt);
void _draw_game(void);


static bool is_game_running = true;
static CollisionBoxList collision_boxes[NUM_COLLISION_TYPES] = {0};
static Sprite *foreground_sprites = NULL;

static Sound game_music = {0};

static Tilemap *map = NULL;

static Player *player = NULL;
static Camera2D *camera = NULL;

static float fire_timer = 0;
static Gun *gun = NULL;
static Bullet *bullet = NULL;
static Bullet bullet_list[MAX_NUM_BULLETS] = {0};

static float spawn_enemy_timer = 0;
static Enemy *enemy_types = NULL;
static EnemyList enemy_list = {0};


void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(120);
    is_game_running = true;
    SetRandomSeed(0);

    game_music = LoadSound("resources/audio/music.wav");

    map = init_tilemap(collision_boxes);

    player = init_player(map->player_initial_pos, collision_boxes);
    camera = init_player_camera(player);
    gun = init_gun(player);

    bullet = init_bullet(collision_boxes);
    bullet_list[0] = instance_bullet(bullet, gun->tip, gun->direction);
    StopSound(*bullet_list[0].shoot_sfx);
    bullet_list[0].is_visible = false;

    enemy_types = init_enemy_types(collision_boxes);

    foreground_sprites = (Sprite*)MemAlloc(sizeof(Sprite) * (map->obj_blocks_size + MAX_NUM_ENEMIES + 1) );
    for (int i=0; i<map->obj_blocks_size; i++) {
        foreground_sprites[i] = map->obj_blocks[i].spr;
    }
    for (int j=0; j<MAX_NUM_ENEMIES; j++) {
        if (enemy_list.list[j].is_visible) {
            foreground_sprites[map->obj_blocks_size + 1 + j] = enemy_list.list[j].spr_anim[0];
        }
    }

    Sprite *player_sprite = &player->spr[0][0];
    foreground_sprites[map->obj_blocks_size] = *player_sprite;

    PlaySound(game_music);
    SetSoundVolume(game_music, .5f);

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

    destroy_collision_box_list(collision_boxes);

    destroy_enemy_types(enemy_types);

    destroy_bullet(bullet);
    destroy_player(player);

    destroy_tilemap(map);

    StopSound(game_music);
    UnloadSound(game_music);

    CloseAudioDevice();
    CloseWindow();

    return;
}

void _update_game(float dt) {
    if (WindowShouldClose()) { is_game_running = false; }

    if (!IsSoundPlaying(game_music)) {
        PlaySound(game_music);
    }

    update_player(player, collision_boxes, dt);
    update_player_camera(camera, player);
    update_gun(gun, player);

    for (int i=0; i<map->obj_blocks_size+1+MAX_NUM_ENEMIES; i++) {
        foreground_sprites[i] = (Sprite) {0};
    }
    for (int i=0; i<map->obj_blocks_size; i++) {
        foreground_sprites[i] = map->obj_blocks[i].spr;
    }
    Sprite current_player_sprite = player->spr[player->facing_direction][player->current_frame];
    foreground_sprites[map->obj_blocks_size] = current_player_sprite;
    for (int j=0; j<MAX_NUM_ENEMIES; j++) {
        if (enemy_list.list[j].is_visible) {
            Sprite current_enemy_sprite = enemy_list.list[j].spr_anim[enemy_list.list[j].current_frame];
            foreground_sprites[map->obj_blocks_size + 1 + j] = current_enemy_sprite;
        }
    }

    if (IsKeyDown(KEY_SPACE) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        for (int i=0; i<MAX_NUM_BULLETS; i++) {
            if(!bullet_list[i].is_visible && fire_timer <= 0) {
                bullet_list[i] = instance_bullet(bullet, gun->tip, gun->direction);
                fire_timer = .35f;
                break;
            }
        }
    }
    fire_timer -= dt;
    update_bullet_list(bullet_list, MAX_NUM_BULLETS, collision_boxes, dt);

    if (spawn_enemy_timer <= 0) {
        spawn_enemy_timer = 3.f;
        for (int i=0; i<MAX_NUM_ENEMIES; i++) {
            if (!enemy_list.list[i].is_visible) {
                int rand_enemy = GetRandomValue(0, NUM_ENEMY_TYPES-1);
                int rand_spwn_pnt = GetRandomValue(0, map->num_enemy_spawn_points - 1);
                enemy_list.list[i] = instance_enemy(&enemy_types[rand_enemy], map->enemy_spawn_points[rand_spwn_pnt]);
                enemy_list.num++;
                break;
            }
        }
    }
    spawn_enemy_timer -= dt;
    update_enemy_list(enemy_list.list, MAX_NUM_ENEMIES, player->position, collision_boxes, dt);

    return;
}
void _draw_game(void) {
    BeginDrawing();
    BeginMode2D(*camera);
    ClearBackground(RAYWHITE);

    draw_tilemap(map);
    sort_and_draw_sprite_list(foreground_sprites, map->obj_blocks_size + 1 + MAX_NUM_ENEMIES);
    draw_bullet_list(bullet_list, MAX_NUM_BULLETS);
    draw_gun(gun);

    EndMode2D();
    EndDrawing();

    return;
}
