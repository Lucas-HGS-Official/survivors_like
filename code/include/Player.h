#pragma once

#include <raylib.h>

typedef struct Player Player;

Player *init_player(void);
void update_player(Player *player,float dt);
void draw_player(Player *player);
void destroy_player(Player *player);
