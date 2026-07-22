#pragma once

#include <raylib.h>

#include "Sprite.h"
#include "CollisionBoxes.h"
#include "settings.h"


typedef enum PlayerAnimState {
    STOPPED_PLAYER,
    WALKING_PLAYER,

    NUM_ANIM_STATES,
} PlayerAnimState;

typedef enum PlayerFacingDir {
    DOWN_FACE_PLAYER,
    UP_FACE_PLAYER,
    RIGHT_FACE_PLAYER,
    LEFT_FACE_PLAYER,

    NUM_FACE_PLAYER,
} PlayerFacingDir;

typedef struct Player {
    Sprite spr[NUM_FACE_PLAYER][NUM_FRAMES];
    Vector2 position;
    Vector2 direction;
    float speed;
    Rectangle hitbox_rec;

    PlayerFacingDir facing_direction;
    PlayerAnimState anim_state;
    float frame_timer;
    float current_frame_time;
    int current_frame;
    Color tint;
} Player;


Player *init_player(Vector2 initial_pos);
Camera2D *init_player_camera(Player *player);
void update_player(Player *player, float dt);//, CollisionBoxList **collision_recs_list, float dt);
void update_player_camera(Camera2D *camera, Player *player);
void draw_player(Player *player);
void destroy_player(Player *player);
