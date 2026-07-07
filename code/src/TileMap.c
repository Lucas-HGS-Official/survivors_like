#include "TileMap.h"

#include <assert.h>
#include <raylib.h>
#define CUTE_TILED_IMPLEMENTATION
#include "cute_tiled.h"

#include "Sprite.h"


typedef enum ObjectsGID {
    GRASSROCK1 = 211,
    GRASSROCK2,
    GREEN_TREE,
    GREEN_TREE_BUSHY,
    GREEN_TREE_SMALL,
    ICE_TREE,
    RUIN_PILLAR,
    RUIN_PILLAR_BROKE,
    RUIN_PILLAR_BROKE_ALT,
    SANDROCK1,
    SANDROCK2,
    PALM,
    PALM_ALT,
    PALM_SMALL,

    OBJECTS_GID_NUM = PALM_SMALL-GRASSROCK1 +1,
} ObjectsGID;


typedef struct Tilemap {
    Sprite tileset;
    cute_tiled_map_t *tilemap;

    cute_tiled_layer_t *layer;
    int *tilemap_data;

    cute_tiled_tileset_t *tileset_data;
    int cols;
} Tilemap;

typedef struct MapObj {
    Sprite spr;
    ObjectsGID gid;
    char *type;
} MapObj;

typedef struct MapObjBlock {
    Rectangle dest_rec;
    ObjectsGID gid;
} MapObjBlock;


static MapObj map_obj_list[OBJECTS_GID_NUM] = {0};
static MapObjBlock *map_obj_block_list = NULL;
static int map_obj_block_list_size = 0;


Tilemap *init_tilemap(void) {
    Tilemap *map = (Tilemap*)MemAlloc(sizeof(Tilemap));

    map->tilemap = cute_tiled_load_map_from_file("resources/data/maps/world.json", NULL);
    assert(map->tilemap);

    map->layer = map->tilemap->layers;
    map->tilemap_data = map->layer->data;
    map->tileset_data = map->tilemap->tilesets;
    map->cols = map->tileset_data->columns;

    init_sprite(&map->tileset, "resources/data/graphics/tilesets/world_tileset.png");
    map->tileset.origin = (Vector2) {0};
    map->tileset.src_rec = (Rectangle) {
        .x = 0, .y = 0,
        .width = map->tileset_data->tilewidth, .height = map->tileset_data->tileheight,
    };
    map->tileset.dest_rec = map->tileset.src_rec;

    char *map_obj_type_list[OBJECTS_GID_NUM] = {
        "grassrock1",
        "grassrock2",
        "green_tree",
        "green_tree_bushy",
        "green_tree_small",
        "ice_tree",
        "ruin_pillar",
        "ruin_pillar_broke",
        "ruin_pillar_broke_alt",
        "sandrock1",
        "sandrock2",
        "palm",
        "palm_alt",
        "palm_small",
    };

    for (int i=0; i<OBJECTS_GID_NUM; i++) {
        map_obj_list[i].gid = i + GRASSROCK1;
        map_obj_list[i].type = map_obj_type_list[i];
        init_sprite(&map_obj_list[i].spr, (char*)TextFormat("resources/data/graphics/objects/%s.png", map_obj_list[i].type));
        map_obj_list[i].spr.origin = (Vector2) {0};
    }

    cute_tiled_layer_t *current_layer = map->layer;
    while (current_layer) {
        if (TextIsEqual("objectgroup", current_layer->type.ptr)) {
            if (TextIsEqual("Objects", current_layer->name.ptr)) {
                int i=0;
                cute_tiled_object_t *current_obj = current_layer->objects;
                while (current_obj) {
                    i++;
                    current_obj = current_obj->next;
                }

                map_obj_block_list = (MapObjBlock*)MemAlloc(sizeof(MapObjBlock) * i);
                map_obj_block_list_size = i;
                cute_tiled_object_t *obj = current_layer->objects;
                for (i--; i>=0; i--) {
                    map_obj_block_list[i].gid = obj->gid;
                    map_obj_block_list[i].dest_rec = (Rectangle) {
                        .x = obj->x, .y = obj->y,
                        .width = obj->width, .height = obj->height,
                    };

                    obj = obj->next;
                }
            }
        }
        current_layer = current_layer->next;
    }

    return map;
}

void update_tilemap(Tilemap *map) {

    return;
}

void draw_tilemap(Tilemap *map) {
    cute_tiled_layer_t *current_layer = map->layer;

    // current_layer->type.ptr
    // tilelayer  0
    // objectgroup  1
    // objectgroup  2
    // objectgroup  3
    while (current_layer) {
        if(TextIsEqual("tilelayer", current_layer->type.ptr)) {
            for (int i=0; i<current_layer->data_count; i++) {
                // Picking the tile from the tileset
                int tile_id = map->tilemap_data[i]-1;
                int tile_col = tile_id % map->cols;
                int tile_row = tile_id / map->cols;
                map->tileset.src_rec.x = map->tileset.src_rec.width * tile_col;
                map->tileset.src_rec.y = map->tileset.src_rec.height * tile_row;

                // Placing the tile in the tilemap grid
                int map_current_col = i % map->layer->width;
                int map_current_row = i / map->layer->width;
                map->tileset.dest_rec.x = map->tileset.dest_rec.width * map_current_col;
                map->tileset.dest_rec.y = map->tileset.dest_rec.height * map_current_row;

                draw_sprite(&map->tileset, WHITE);
            }
        } else if (TextIsEqual("objectgroup", current_layer->type.ptr)) {
            // current_layer->name.ptr
            // Entities
            // Collisions
            // Objects
            if (TextIsEqual("Objects", current_layer->name.ptr)) {
                for (int i=0; i<map_obj_block_list_size; i++) {
                    for (int j=0; j<OBJECTS_GID_NUM; j++) {
                        if (map_obj_list[j].gid == map_obj_block_list[i].gid) {
                            map_obj_list[j].spr.dest_rec = map_obj_block_list[i].dest_rec;
                            map_obj_list[j].spr.dest_rec.y -= map_obj_list[j].spr.dest_rec.height;

                            draw_sprite(&map_obj_list[j].spr, WHITE);
                        }
                    }
                }
            }
        }
        current_layer = current_layer->next;
    }

    return;
}

void destroy_tilemap(Tilemap *map) {
    for (int i=0; i<OBJECTS_GID_NUM; i++) {
        destroy_sprite(&map_obj_list[i].spr);
    }
    MemFree(map_obj_block_list);
    destroy_sprite(&map->tileset);
    cute_tiled_free_map(map->tilemap);
    MemFree(map);

    return;
}
