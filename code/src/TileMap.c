#include "TileMap.h"

#include <assert.h>
#include <raylib.h>
#include "cute_tiled.h"
#define CUTE_TILED_IMPLEMENTATION

#include "Sprite.h"


typedef struct Tile {
    int id;
} Tile;

typedef struct Tilemap {
    Sprite tileset;
    cute_tiled_map_t *tilemap;

    cute_tiled_layer_t *layer;
    int *tilemap_data;

    cute_tiled_tileset_t *tileset_data;
    int cols;
    int rows;


    Tile current_tile;
} Tilemap;


Tilemap *init_tilemap(void) {
    Tilemap *map = (Tilemap*)MemAlloc(sizeof(Tilemap));

    map->tilemap = cute_tiled_load_map_from_file("resources/data/maps/world.json", NULL);
    assert(map->tilemap);

    map->layer = map->tilemap->layers;
    map->tilemap_data = map->layer->data;
    map->tileset_data = map->tilemap->tilesets;
    map->cols = map->tileset_data->columns;
    map->rows = map->tileset_data->tilecount/map->cols;

    map->current_tile.id = *map->tilemap_data;

    init_sprite(&map->tileset, "resources/data/graphics/tilesets/world_tileset.png");
    map->tileset.origin = (Vector2) {0};
    map->tileset.src_rec = (Rectangle) {
        .x = 0, .y = 0,
        .width = map->tileset_data->tilewidth, .height = map->tileset_data->tileheight,
    };
    map->tileset.dest_rec = map->tileset.src_rec;

    return map;
}

void update_tilemap(Tilemap *map) {

    return;
}

void draw_tilemap(Tilemap *map) {

    draw_sprite(&map->tileset, WHITE);

    return;
}

void destroy_tilemap(Tilemap *map) {
    destroy_sprite(&map->tileset);
    cute_tiled_free_map(map->tilemap);
    MemFree(map);

    return;
}
