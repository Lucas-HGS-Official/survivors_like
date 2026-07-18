#include "CollisionBoxes.h"


CollisionBoxList *create_collision_box_list(Rectangle *recs, CollisionType box_type, int recs_num) {
    CollisionBoxList *collision_recs_list = (CollisionBoxList*) MemAlloc(sizeof(CollisionBoxList));
    collision_recs_list->size = recs_num;

    collision_recs_list->box_list = (CollisionBox*) MemAlloc(sizeof(CollisionBox) * recs_num);

    for (int i=0; i<collision_recs_list->size; i++) {
        collision_recs_list->box_list[i].rec = recs[i];
        collision_recs_list->box_list[i].type = box_type;
    }

    return collision_recs_list;
}

// CollisionBox check_collision_box_list(CollisionBox box, CollisionBoxList box_list) {


// }

void destroy_collision_box_list(CollisionBoxList *collision_recs_list) {
    MemFree(collision_recs_list->box_list);
    MemFree(collision_recs_list);

    return;
}
