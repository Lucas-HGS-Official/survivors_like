#pragma once

#include <raylib.h>

#include "Sprite.h"
#include "Player.h"


typedef struct Gun {
    Sprite spr;
    float distance;
    Rectangle src_rec;
    Rectangle src_rec_flip;
    Vector2 direction;
    Vector2 tip;
} Gun;


Gun *init_gun(Player *player);
void update_gun(Gun *gun, Player *player);
void draw_gun(Gun *gun);
void destroy_gun(Gun *gun);
