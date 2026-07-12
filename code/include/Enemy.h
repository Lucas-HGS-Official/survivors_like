#pragma once

#include "Sprite.h"
#include <raylib.h>


typedef enum ENEMY_TYPES {
    BAT,
    BLOB,
    SKELETON,

    NUM_ENEMY_TYPES,
} ENEMY_TYPES;

typedef struct EnemyTypes {
    Sprite **spr_list;
} EnemyTypes;

typedef struct Enemy {
    Sprite *spr_anim;
    Vector2 direction;
    float speed;
} Enemy;


EnemyTypes *init_enemy_types(void);
Enemy instance_enemy(Sprite *enemy_sprites);
void update_enemy_list(Enemy *enemy_list);
void destroy_enemy_types(EnemyTypes *enemy_types);
