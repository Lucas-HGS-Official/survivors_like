#include "Player.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "settings.h"
#include "Sprite.h"


#define HORIZONTAL_WHITE_SPACE_PLAYER_SPR 120

#define HORIZONTAL_COLLISION_MODE 'h'
#define VERTICAL_COLLISION_MODE 'v'


void _load_player_sprites(Player *player);

void _start_animation(Player *player);
void _stop_animation(Player *player);

void _controls(Player *player);
void _movement(Player *player, CollisionBoxList *collision_recs_list, float dt);
void _collision(Player *player, char collision_mode, CollisionBoxList *collision_rec_list, int num_recs);


Player *init_player(Vector2 initial_pos) {
    Player *player = (Player*) MemAlloc(sizeof(Player));

    _load_player_sprites(player);

    player->direction = (Vector2) {0};
    player->speed = 400.f;
    player->position = initial_pos;

    Sprite *current_sprite = &player->spr[player->facing_direction][player->current_frame];
    current_sprite->dest_rec.x = initial_pos.x;
    current_sprite->dest_rec.y = initial_pos.y;
    Rectangle player_hitbox_rec = current_sprite->dest_rec;
    player_hitbox_rec.x += HORIZONTAL_WHITE_SPACE_PLAYER_SPR/4.f;
    player_hitbox_rec.y += current_sprite->dest_rec.height/2.f;
    player_hitbox_rec.width -= HORIZONTAL_WHITE_SPACE_PLAYER_SPR/2.f;
    player_hitbox_rec.height -= current_sprite->dest_rec.height/2.f;
    player->hitbox_rec = player_hitbox_rec;

    return player;
}
Camera2D *init_player_camera(Player *player) {
    Camera2D *camera = (Camera2D*)MemAlloc(sizeof(Camera2D));
    camera->target.x = player->position.x + player->spr[0]->dest_rec.width/2.f;
    camera->target.y = player->position.y + player->spr[0]->dest_rec.height/2.f;
    camera->offset = (Vector2){ .x=WINDOW_WIDTH/2.0f, .y=WINDOW_HEIGHT/2.0f };
    camera->rotation = 0.0f;
    camera->zoom = 1.0f;

    return camera;
}
void update_player(Player *player, CollisionBoxList *collision_recs_list,float dt) {
    _controls(player);

    // Animation state machine
    switch (player->anim_state) {
        case STOPPED_PLAYER:
            player->current_frame = 0;
            player->frame_timer = 0;
            break;
        case WALKING_PLAYER:
            if (player->frame_timer <= GetTime() - player->current_frame_time) {
                player->current_frame += 1;
                if (player->current_frame >= NUM_FRAMES) {
                    player->current_frame = 0;
                }
                player->frame_timer = GetTime();
            }
            break;
        case NUM_ANIM_STATES:
            break;
    }

    _movement(player, collision_recs_list, dt);

    return;
}
void update_player_camera(Camera2D *camera, Player *player) {
    camera->target.x = player->position.x + player->spr[0]->dest_rec.width/2.f;
    camera->target.y = player->position.y + player->spr[0]->dest_rec.height/2.f;

    return;
}
void draw_player(Player *player) {
    Sprite *current_sprite = &player->spr[player->facing_direction][player->current_frame];
    draw_sprite(current_sprite, player->tint);
    // DrawRectangleLinesEx(current_sprite->dest_rec, 2.f, RED);
    // DrawRectangleLinesEx(player->hitbox_rec, 3.f, BLUE);

    return;
}
void destroy_player(Player* player) {
    // Unloading each texture
    for (int i=0; i<NUM_FACE_PLAYER; i++) {
        for (int j=0; j<NUM_FRAMES; j++) {
            destroy_sprite(&player->spr[i][j]);
        }
    }
    MemFree(player);

    return;
}

void _collision(Player *player, char collision_mode, CollisionBoxList *collision_rec_list, int num_recs) {
    if (collision_rec_list == NULL) {
        return;
    }
    Rectangle player_hitbox_rec = player->hitbox_rec;
    for (int i=0; i<num_recs; i++) {
        if (CheckCollisionRecs(player_hitbox_rec, collision_rec_list->box_list[i].rec)) {

            float collided_right_side = collision_rec_list->box_list[i].rec.x + collision_rec_list->box_list[i].rec.width;
            float collided_left_left = collision_rec_list->box_list[i].rec.x;
            float collided_top_side = collision_rec_list->box_list[i].rec.y;
            float collided_bottom_side = collision_rec_list->box_list[i].rec.y + collision_rec_list->box_list[i].rec.height;


            if (collision_mode == 'h') {
                if (player->direction.x > 0) {
                    player->hitbox_rec.x = collided_left_left - player_hitbox_rec.width;
                }
                if (player->direction.x < 0) {
                    player->hitbox_rec.x = collided_right_side;
                }
            } else {
                if (player->direction.y > 0) {
                    player->hitbox_rec.y = collided_top_side - player_hitbox_rec.height;
                }
                if (player->direction.y < 0) {
                    player->hitbox_rec.y = collided_bottom_side;
                }
            }
        }
    }
}

void _start_animation(Player *player) {
    player->anim_state = WALKING_PLAYER;
    if (player->frame_timer == 0) { player->frame_timer = GetTime(); }

    return;
}
void _stop_animation(Player *player) {
    player->anim_state = STOPPED_PLAYER;
    player->frame_timer = 0;

    return;
}

void _controls(Player *player) {
    float up_move = (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W));
    float down_move = (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S));
    float right_move = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D));
    float left_move = (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A));
    player->direction = (Vector2){
        .x = right_move - left_move,
        .y = down_move - up_move,
    };
    player->direction = Vector2Normalize(player->direction);


    if (!down_move && !up_move) { player->direction.y = 0; }
    if (!right_move && !left_move) { player->direction.x = 0; }

    player->direction = Vector2Normalize(player->direction);

    if (player->direction.x > 0) { player->facing_direction = RIGHT_FACE_PLAYER; }
    if (player->direction.x < 0) { player->facing_direction = LEFT_FACE_PLAYER; }
    if (player->direction.y > 0) { player->facing_direction = DOWN_FACE_PLAYER; }
    if (player->direction.y < 0) { player->facing_direction = UP_FACE_PLAYER; }

    if (player->direction.y == 0 && player->direction.x == 0) {
        _stop_animation(player);
    } else { _start_animation(player); }


    return;
}

void _movement(Player *player, CollisionBoxList *collision_recs_list, float dt) {
    // Player movement
    Sprite *current_sprite = &player->spr[player->facing_direction][player->current_frame];

    player->hitbox_rec.x += player->direction.x * player->speed * dt;
    _collision(player, HORIZONTAL_COLLISION_MODE, collision_recs_list, collision_recs_list->size);
    player->hitbox_rec.y += player->direction.y * player->speed * dt;
    _collision(player, VERTICAL_COLLISION_MODE, collision_recs_list, collision_recs_list->size);

    player->position = (Vector2) {
        .x = player->hitbox_rec.x - HORIZONTAL_WHITE_SPACE_PLAYER_SPR/4.f,
        .y = player->hitbox_rec.y - current_sprite->dest_rec.height/2.f,
    };

    current_sprite->dest_rec.x = player->position.x;
    current_sprite->dest_rec.y = player->position.y;

    return;
}

void _load_player_sprites(Player *player) {
    // Loading all images for each player frame
    for (int i=0; i<NUM_FACE_PLAYER; i++) {
        char *facing_dir = "";
        switch (i) {
            case DOWN_FACE_PLAYER:
                facing_dir = "down";
                break;
            case UP_FACE_PLAYER:
                facing_dir = "up";
                break;
            case RIGHT_FACE_PLAYER:
                facing_dir = "right";
                break;
            case LEFT_FACE_PLAYER:
                facing_dir = "left";
                break;
        }
        for (int j=0; j<NUM_FRAMES; j++) {
            char *filepath = (char*) TextFormat("resources/images/player/%s/%i.png", facing_dir, j);
            init_sprite(&(player->spr[i][j]), filepath);
        }
    }

    // Initiating all other player values
    player->facing_direction = DOWN_FACE_PLAYER;
    player->anim_state = WALKING_PLAYER;
    player->current_frame = 0;
    player->current_frame_time = 0.1f;
    player->frame_timer = 0;
    player->tint = WHITE;

    return;
}
