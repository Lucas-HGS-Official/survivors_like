#pragma once

#include <raylib.h>
#include <stdbool.h>

#include "Sprite.h"


#define BULLET_LIST_SIZE 100


typedef struct Bullet {
    bool is_visible;
    Vector2 direction;
    Sprite spr;
    float speed;
    float lifetime;
} Bullet;


Bullet *init_bullet(void);
Bullet instance_bullet(Bullet *bullet, Vector2 spawn_pos, Vector2 spawn_direction);
void update_bullet_list(Bullet *bullet_list, int bullet_list_size, float dt);
void draw_bullet_list(Bullet *bullet_list, int bullet_list_size);
void destroy_bullet(Bullet *bullet);
