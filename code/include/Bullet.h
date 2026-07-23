#pragma once

#include <raylib.h>
#include <stdbool.h>

#include "CollisionBoxes.h"
#include "Sprite.h"


#define MAX_NUM_BULLETS 100


typedef struct Bullet {
    bool is_visible;
    bool is_marked_for_deletion;
    Vector2 direction;
    Sprite spr;
    float speed;
    float lifetime;
} Bullet;


Bullet *init_bullet(CollisionBoxList *collision_boxes);
Bullet instance_bullet(Bullet *bullet, Vector2 spawn_pos, Vector2 spawn_direction);
void update_bullet_list(Bullet *bullet_list, int bullet_list_size, CollisionBoxList *collision_boxes, float dt);
void draw_bullet_list(Bullet *bullet_list, int bullet_list_size);
void destroy_bullet(Bullet *bullet);
