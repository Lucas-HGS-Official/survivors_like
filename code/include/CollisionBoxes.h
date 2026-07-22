#pragma once

#include <raylib.h>
#include <stdbool.h>


// typedef enum CollisionType {
//     NONE_COLLISION_TYPE,

//     ENV_COLLISION_TYPE,
//     ENEMY_COLLISION_TYPE,
//     PLAYER_COLLISION_TYPE,
//     BULLET_COLLISION_TYPE,

//     NUM_COLLISION_TYPES,
// } CollisionType;

// typedef struct CollisionBox {
//     Rectangle rec;
//     CollisionType type;
// } CollisionBox;

// typedef struct CollisionBoxList {
//     CollisionBox *list;
//     int size;
// } CollisionBoxList;


// CollisionBoxList *create_collision_box_list(Rectangle *recs, CollisionType box_type, int recs_num);
// CollisionBox check_collision_box_list(CollisionBox box, CollisionBoxList **box_list);
// void destroy_collision_box_list(CollisionBoxList *collision_recs_list);
