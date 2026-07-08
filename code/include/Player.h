#pragma once

#include <raylib.h>

#include "CollisionBoxes.h"


typedef struct Player Player;


Player *init_player(Vector2 initial_pos);
Camera2D *init_player_camera(Player *player);
void update_player(Player *player, CollisionRecs *collision_recs_list,float dt);
void update_player_camera(Camera2D *camera, Player *player);
void draw_player(Player *player);
void destroy_player(Player *player);
