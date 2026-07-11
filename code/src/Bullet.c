#include "Bullet.h"

#include <raylib.h>
#include <stdbool.h>

#include "Sprite.h"


Bullet *init_bullet(void){
    Bullet *bullet = (Bullet*)MemAlloc(sizeof(Bullet));
    init_sprite(&bullet->spr, "resources/images/gun/bullet.png");
    bullet->is_visible = false;
    bullet->speed = 600.f;
    bullet->direction = (Vector2) { .x=1, .y=0 };
    bullet->lifetime = 10.f;

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
void update_bullet_list(Bullet *bullet_list, int bullet_list_size, float dt) {
    for (int i=0; i<bullet_list_size; i++) {
        if (bullet_list[i].is_visible) {
            if (bullet_list[i].lifetime <= 0) {
                bullet_list[i].is_visible = false;
            }
            bullet_list[i].lifetime -= dt;
            bullet_list[i].spr.dest_rec.x += bullet_list[i].direction.x * bullet_list[i].speed * dt;
            bullet_list[i].spr.dest_rec.y += bullet_list[i].direction.y * bullet_list[i].speed * dt;
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
