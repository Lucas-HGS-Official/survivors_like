#pragma once

#include "cute_tiled.h"
#include <raylib.h>


typedef struct Tilemap Tilemap;


Tilemap *init_tilemap(void);
void update_tilemap(Tilemap *map);
void draw_tilemap(Tilemap *map);
void destroy_tilemap(Tilemap *map);
// Rectangle *get_obj_block_collision_recs(Rectangle *get_obj_block_collision_recs(Tilemap *map));
