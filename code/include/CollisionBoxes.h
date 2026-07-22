#pragma once

#include <raylib.h>
#include <stdbool.h>


typedef enum CollisionType {
    NONE_COLLISION_TYPE,

    ENV_COLLISION_TYPE,
    ENEMY_COLLISION_TYPE,
    PLAYER_COLLISION_TYPE,
    BULLET_COLLISION_TYPE,

    NUM_COLLISION_TYPES,
} CollisionType;

typedef struct CollisionBox {
    Rectangle rec;
    CollisionType type;
} CollisionBox;

typedef struct CollisionBoxList {
    int size;
    CollisionType type;
    Rectangle *list;
} CollisionBoxList;


void create_collision_box_list(CollisionBoxList *collision_boxes, Rectangle *recs, CollisionType box_type, int num_recs);
CollisionBox check_collision_box_list(CollisionBox box, CollisionBoxList *collision_boxes);
void destroy_collision_box_list(CollisionBoxList *collision_boxes);
