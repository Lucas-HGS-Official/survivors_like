#pragma once

#include <raylib.h>


typedef enum CollisionType {
    ENV_COLLISION_TYPE,
    ENEMY_COLLISION_TYPE,
    PLAYER_COLLISION_TYPE,
    BULLET_COLLISION_TYPE,
} CollisionType;

typedef struct CollisionBox {
    Rectangle rec;
    CollisionType type;
} CollisionBox;

typedef struct CollisionRecs {
    CollisionBox *box_list;
    int num;
} CollisionRecs;


CollisionRecs *create_collision_recs_list(Rectangle *recs, CollisionType box_type, int recs_num);
void destroy_collision_recs_list(CollisionRecs *collision_recs_list);
