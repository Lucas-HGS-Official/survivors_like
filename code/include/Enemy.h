#pragma once

#include "CollisionBoxes.h"
#include "Sprite.h"
#include "settings.h"
#include <raylib.h>


#define MAX_NUM_ENEMIES 100


typedef enum ENEMY_TYPES {
    BAT,
    BLOB,
    SKELETON,

    NUM_ENEMY_TYPES,
} ENEMY_TYPES;

typedef struct Enemy {
    bool is_visible;
    Sprite spr_anim[NUM_FRAMES];
    Vector2 position;
    Vector2 direction;
    float speed;
    Rectangle hitbox_rec;
    float frame_timer;
    int current_frame;
} Enemy;


Enemy *init_enemy_types(void);
Enemy instance_enemy(Enemy *enemy, Vector2 spawn_point);
void update_enemy_list(Enemy *enemy_list, int enemy_list_size, Vector2 player_position, CollisionBoxList *collision_recs_list, float dt);
void draw_enemy_list(Enemy *enemy_list, int enemy_list_size);
void destroy_enemy_types(Enemy *enemy_types);
