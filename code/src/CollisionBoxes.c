#include "CollisionBoxes.h"


CollisionRecs *create_collision_recs_list(Rectangle *recs, int recs_num) {
    CollisionRecs *collision_recs_list = (CollisionRecs*) MemAlloc(sizeof(CollisionRecs));
    collision_recs_list->num = recs_num;

    collision_recs_list->box_list = (CollisionBox*) MemAlloc(sizeof(Rectangle) * recs_num);

    for (int i=0; i<collision_recs_list->num; i++) {
        collision_recs_list->box_list[i].rec = recs[i];
        collision_recs_list->box_list[i].type;
    }

    return collision_recs_list;
}

void destroy_collision_recs_list(CollisionRecs *collision_recs_list) {
    MemFree(collision_recs_list->box_list);
    MemFree(collision_recs_list);

    return;
}
