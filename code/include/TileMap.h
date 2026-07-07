#pragma once

#include "cute_tiled.h"
#include <raylib.h>

#include "Sprite.h"
#include "CollisionBoxes.h"


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


typedef struct MapObj {
    Sprite spr;
    ObjectsGID gid;
    char *type;
} MapObj;

typedef struct MapObjBlock {
    Rectangle dest_rec;
    ObjectsGID gid;
} MapObjBlock;

typedef struct Tilemap {
    Sprite tileset;
    cute_tiled_map_t *tilemap;

    cute_tiled_layer_t *layer;
    int *tilemap_data;

    cute_tiled_tileset_t *tileset_data;
    int cols;

    MapObj obj_types[OBJECTS_GID_NUM];
    MapObjBlock *obj_blocks;
    int obj_blocks_size;
    CollisionRecs *obj_blocks_hitboxes;
} Tilemap;


Tilemap *init_tilemap(void);
void update_tilemap(Tilemap *map);
void draw_tilemap(Tilemap *map);
void destroy_tilemap(Tilemap *map);
