#pragma once

#include <raylib.h>

#include "Sprite.h"

typedef struct Player {
    Sprite spr;
    Vector2 direction;
    float speed;
} Player;

void init_player(Player *player);
void update_player(Player *player,float dt);
void destroy_player(Player *player);
