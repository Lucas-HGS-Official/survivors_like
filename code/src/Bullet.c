#include "Bullet.h"

#include <raylib.h>
#include <stdbool.h>

#include "CollisionBoxes.h"
#include "Sprite.h"


Bullet *init_bullet(CollisionBoxList *collision_boxes) {
    Bullet *bullet = (Bullet*)MemAlloc(sizeof(Bullet));
    init_sprite(&bullet->spr, "resources/images/gun/bullet.png");
    bullet->is_visible = false;
    bullet->speed = 600.f;
    bullet->direction = (Vector2) { .x=1, .y=0 };
    bullet->lifetime = 10.f;

    collision_boxes[BULLET_COLLISION_TYPE].type = BULLET_COLLISION_TYPE;
    collision_boxes[BULLET_COLLISION_TYPE].size = MAX_NUM_BULLETS;
    collision_boxes[BULLET_COLLISION_TYPE].list = (Rectangle*)MemAlloc(sizeof(Rectangle) * MAX_NUM_BULLETS);
    for (int i=0; i<MAX_NUM_BULLETS; i++) {
        collision_boxes[BULLET_COLLISION_TYPE].list[i] = (Rectangle) { 0 };
    }

    return bullet;
}
Bullet instance_bullet(Bullet *bullet, Vector2 spawn_pos, Vector2 spawn_direction){
    Bullet new_bullet = *bullet;
    new_bullet.direction = spawn_direction;
    bullet->is_visible = true;
    new_bullet.spr.dest_rec.x = spawn_pos.x;
    new_bullet.spr.dest_rec.y = spawn_pos.y;

    return new_bullet;
}
void update_bullet_list(Bullet *bullet_list, int bullet_list_size, CollisionBoxList *collision_boxes, float dt) {
    for (int i=0; i<bullet_list_size; i++) {
        collision_boxes[BULLET_COLLISION_TYPE].list[i] = (Rectangle) {0};
        if (bullet_list[i].is_visible) {
            if (bullet_list[i].lifetime <= 0) {
                bullet_list[i].is_visible = false;
            }
            bullet_list[i].lifetime -= dt;
            bullet_list[i].spr.dest_rec.x += bullet_list[i].direction.x * bullet_list[i].speed * dt;
            bullet_list[i].spr.dest_rec.y += bullet_list[i].direction.y * bullet_list[i].speed * dt;

            collision_boxes[BULLET_COLLISION_TYPE].list[i] = bullet_list[i].spr.dest_rec;
        }
    }

    return;
}
void draw_bullet_list(Bullet *bullet_list, int bullet_list_size) {
    for (int i=0; i<bullet_list_size; i++) {
        if (bullet_list[i].is_visible) {
            draw_sprite(&bullet_list[i].spr, WHITE);
        }
    }

    return;
}
void destroy_bullet(Bullet *bullet) {
    destroy_sprite(&bullet->spr);
    MemFree(bullet);

    return;
}
