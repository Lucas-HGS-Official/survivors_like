#include "TileMap.h"

#include <assert.h>
#include <raylib.h>

#include "cute_tiled.h"
#define CUTE_TILED_IMPLEMENTATION


cute_tiled_map_t *init_tilemap(void) {
    cute_tiled_map_t *map = (cute_tiled_map_t*)MemAlloc(sizeof(cute_tiled_map_t));
    map = cute_tiled_load_map_from_file("resources/data/maps/world.json", NULL);
    assert(map);

    return map;
}

void destroy_tilemap(cute_tiled_map_t *map) {
    cute_tiled_free_map(map);

    return;
}
