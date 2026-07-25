#pragma once

#include "CollisionBoxes.h"
#include "Sprite.h"
#include "settings.h"
#include <raylib.h>


#define MAX_NUM_ENEMIES 100


typedef enum EnemyTypes {
    BAT,
    BLOB,
    SKELETON,

    NUM_ENEMY_TYPES,
} EnemyTypes;

typedef enum EnemyStates {
    ENEMY_ALIVE_STATE,
    ENEMY_DEAD_STATE,
} EnemyStates;

typedef struct Enemy {
    bool is_visible;
    bool is_marked_for_deletion;
    Sprite spr_anim[NUM_FRAMES];
    Sprite spr_death;
    Vector2 position;
    Vector2 direction;
    float speed;
    Rectangle hitbox_rec;
    float frame_timer;
    int current_frame;
    Sound *impact_sfx;
} Enemy;

typedef struct EnemyList {
    Enemy list[MAX_NUM_ENEMIES];
    int num;
} EnemyList;


Enemy *init_enemy_types(CollisionBoxList *collision_boxes);
Enemy instance_enemy(Enemy *enemy, Vector2 spawn_point);
void update_enemy_list(Enemy *enemy_list, int enemy_list_size, Vector2 player_position, CollisionBoxList *collision_boxes, float dt);
void draw_enemy_list(Enemy *enemy_list, int enemy_list_size);
void destroy_enemy_types(Enemy *enemy_types);
