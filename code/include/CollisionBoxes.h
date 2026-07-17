#pragma once

#include <raylib.h>

typedef struct CollisionBox {
    Rectangle rec;
    int type;
} CollisionBox;

typedef struct CollisionRecs {
    CollisionBox *box_list;
    int num;
} CollisionRecs;


CollisionRecs *create_collision_recs_list(Rectangle *recs, int recs_num);
void destroy_collision_recs_list(CollisionRecs *collision_recs_list);
