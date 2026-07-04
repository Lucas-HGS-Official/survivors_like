#pragma once

#include "cute_tiled.h"
#define CUTE_TILED_IMPLEMENTATION
#include <raylib.h>


cute_tiled_map_t *init_tilemap(void);
void destroy_tilemap(cute_tiled_map_t *map);
