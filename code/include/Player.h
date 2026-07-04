#pragma once

#include <raylib.h>

#include "CollisionBoxes.h"


typedef struct Player Player;


Player *init_player(void);
void update_player(Player *player, CollisionRecs *collision_recs_list,float dt);
void draw_player(Player *player);
void destroy_player(Player *player);
