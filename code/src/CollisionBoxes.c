#include "CollisionBoxes.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>


bool _is_collision_box_identical(CollisionBox box1, CollisionBox box2);

void create_collision_box_list(CollisionBoxList *collision_boxes, Rectangle *recs, CollisionType box_type, int num_recs) {
    collision_boxes[box_type].size = num_recs;
    collision_boxes[box_type].type = box_type;

    collision_boxes[box_type].list = (Rectangle*)MemAlloc(sizeof(Rectangle)*num_recs);

    for (int i=0; i<collision_boxes[box_type].size; i++) {
        collision_boxes[box_type].list[i] = recs[i];
    }
}

CollisionBox check_collision_box_list(CollisionBox box, CollisionBoxList *collision_boxes) {
    CollisionBox collided_box = {0};

    for (int i=0; i<NUM_COLLISION_TYPES; i++) {
        for (int j=0; j<collision_boxes[i].size; j++) {
            collided_box.rec = collision_boxes[i].list[j];
            collided_box.type = collision_boxes[i].type;

            if (collided_box.rec.height == 0.f || collided_box.rec.width == 0.f) {
                continue;
            } else if (_is_collision_box_identical(box, collided_box)) {
                continue;
            } else if (CheckCollisionRecs(box.rec, collided_box.rec)) {
                return collided_box;
            }
        }
    }

    collided_box = (CollisionBox) {0};

    return collided_box;
}

void destroy_collision_box_list(CollisionBoxList *collision_boxes) {
    for (int i=0; i<NUM_COLLISION_TYPES; i++) {
        if (collision_boxes[i].list != NULL) {
            printf("\n atempt at %i \n", i);
            MemFree(collision_boxes[i].list);
            printf("\n success at %i \n", i);
        }
    }

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
