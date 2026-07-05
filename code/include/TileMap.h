#pragma once

#include "cute_tiled.h"
#define CUTE_TILED_IMPLEMENTATION
#include <raylib.h>


typedef struct Tilemap Tilemap;

Tilemap *init_tilemap(void);
void update_tilemap(Tilemap *map);
void draw_tilemap(Tilemap *map);
void destroy_tilemap(Tilemap *map);
