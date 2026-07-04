#pragma once

#include <raylib.h>

#include "Sprite.h"


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
    Sprite **spr;
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


typedef struct CollisionRecs {
    Rectangle *recs;
    int num;
} CollisionRecs;

Player *init_player(void);
void update_player(Player *player, CollisionRecs *collision_recs_list,float dt);
void draw_player(Player *player);
void destroy_player(Player *player);

CollisionRecs *create_collision_recs_list(Rectangle *recs, int recs_num);
void destroy_collision_recs_list(CollisionRecs *collision_recs_list);
