#include "TileMap.h"

#include <assert.h>
#include <raylib.h>
#include "cute_tiled.h"
#define CUTE_TILED_IMPLEMENTATION

#include "Sprite.h"


typedef struct Tilemap {
    Sprite tileset;
    cute_tiled_map_t *tilemap;

    cute_tiled_layer_t *layer;
    int *tilemap_data;

    cute_tiled_tileset_t *tileset_data;
    int cols;
    int rows;
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
    cute_tiled_layer_t *current_layer = map->layer;

    while (current_layer) {
        if(TextIsEqual("Ground", map->layer->name.ptr)) {
            for (int i=0; i<current_layer->data_count; i++) {
                // Picking the tile from the tileset
                int tile_id = map->tilemap_data[i]-1;
                int tile_col = tile_id % map->cols;
                int tile_row = tile_id / map->cols;
                map->tileset.src_rec.x = map->tileset.src_rec.width * tile_col;
                map->tileset.src_rec.y = map->tileset.src_rec.height * tile_row;

                // Placing the tile in the tilemap
                int map_current_col = i % map->layer->width;
                int map_current_row = i / map->layer->width;
                map->tileset.dest_rec.x = map->tileset.dest_rec.width * map_current_col;
                map->tileset.dest_rec.y = map->tileset.dest_rec.height * map_current_row;

                draw_sprite(&map->tileset, WHITE);
            }
        }
        current_layer = current_layer->next;
    }

    return;
}

void destroy_tilemap(Tilemap *map) {
    destroy_sprite(&map->tileset);
    cute_tiled_free_map(map->tilemap);
    MemFree(map);

    return;
}
