#include "CollisionBoxes.h"


bool _is_collision_box_identical(CollisionBox box1, CollisionBox box2);

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

CollisionBox check_collision_box_list(CollisionBox box, CollisionBoxList box_list) {
    CollisionBox collided_box = {0};

    for (int i=0; i<box_list.size; i++) {
        if (_is_collision_box_identical(box, box_list.box_list[i])) {
            return box;
        }
        if (CheckCollisionRecs(box.rec, box_list.box_list[i].rec)) {
            return collided_box;
        }
    }

    return collided_box;
}

void destroy_collision_box_list(CollisionBoxList *collision_recs_list) {
    MemFree(collision_recs_list->box_list);
    MemFree(collision_recs_list);

    return;
}


bool _is_collision_box_identical(CollisionBox box1, CollisionBox box2) {
    if (box1.rec.x != box2.rec.x && box1.rec.y != box2.rec.y) {
        return false;
    }
    if (box1.rec.width != box2.rec.width && box1.rec.height != box2.rec.height) {
        return false;
    }
    if (box1.type != box2.type) {
        return false;
    }
    return true;
}
