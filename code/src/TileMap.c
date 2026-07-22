#include "TileMap.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>
#define CUTE_TILED_IMPLEMENTATION
#include "cute_tiled.h"

#include "Sprite.h"
#include "CollisionBoxes.h"


Rectangle *_alloc_map_collision_recs(Tilemap *map);
void _free_map_collision_recs(Rectangle *collision_recs);
void _load_tiled_tilemap(Tilemap *map);
void _load_tiled_obj_sprites(Tilemap *map);

static int _comp_y_value(const void * elem1, const void * elem2);

Tilemap *init_tilemap(CollisionBoxList *collision_boxes) {
    Tilemap *map = (Tilemap*)MemAlloc(sizeof(Tilemap));

    _load_tiled_tilemap(map);

    _load_tiled_obj_sprites(map);

    map->collission_rec_list_size = 0;

    cute_tiled_layer_t *current_layer = map->layer;
    while (current_layer) {
        if (TextIsEqual("objectgroup", current_layer->type.ptr)) {
            if (TextIsEqual("Objects", current_layer->name.ptr)) {
                int i=0;
                for (
                        cute_tiled_object_t *obj = current_layer->objects;
                        obj;
                        obj = obj->next
                ) {
                    i++;
                }

                map->obj_blocks = (MapObjBlock*)MemAlloc(sizeof(MapObjBlock) * i);
                map->obj_blocks_size = i;
                map->collission_rec_list_size += i;
                cute_tiled_object_t *obj = current_layer->objects;
                for (int i=0; i<map->obj_blocks_size; i++) {
                    map->obj_blocks[i].gid = obj->gid;
                    map->obj_blocks[i].dest_rec = (Rectangle) {
                        .x = obj->x, .y = obj->y - obj->height,
                        .width = obj->width, .height = obj->height,
                    };
                    obj = obj->next;
                }

                qsort(map->obj_blocks, map->obj_blocks_size, sizeof(MapObjBlock), _comp_y_value);

                for (int i=0; i<map->obj_blocks_size; i++) {
                    for (int j=0; j<OBJECTS_GID_NUM; j++) {
                        if (map->obj_types[j].gid == map->obj_blocks[i].gid) {
                            map->obj_types[j].spr.dest_rec = map->obj_blocks[i].dest_rec;
                            map->obj_blocks[i].spr = map->obj_types[j].spr;
                        }
                    }
                }

            } else if (TextIsEqual("Collisions", current_layer->name.ptr)) {
                int i=0;
                for (
                        cute_tiled_object_t *obj = current_layer->objects;
                        obj;
                        obj = obj->next
                    ) {
                    i++;
                }
                map->invisible_recs = (Rectangle*)MemAlloc(sizeof(Rectangle) * i);
                map->invisible_recs_size = i;
                map->collission_rec_list_size += i;
                cute_tiled_object_t *obj = current_layer->objects;
                for (int i=0; i<map->invisible_recs_size; i++) {
                    map->invisible_recs[i] = (Rectangle) {
                        .x=obj->x, .y = obj->y,
                        .width=obj->width, .height=obj->height,
                    };
                    obj = obj->next;
                }
            } else if (TextIsEqual("Entities", current_layer->name.ptr)) {
                int i=0;
                for (
                        cute_tiled_object_t *obj = current_layer->objects;
                        obj;
                        obj = obj->next
                ) {
                    if (TextIsEqual("Enemy", obj->name.ptr)) {
                        i++;
                    }
                    if (TextIsEqual("Player", obj->name.ptr)) {
                        map->player_initial_pos = (Vector2) { .x=obj->x, .y=obj->y, };
                    }
                }
                map->num_enemy_spawn_points = i;
                map->enemy_spawn_points = (Vector2*)MemAlloc(sizeof(Vector2) * (i));
                int j=0;
                for (
                        cute_tiled_object_t *obj = current_layer->objects;
                        obj;
                        obj = obj->next
                ) {
                    if (TextIsEqual("Enemy", obj->name.ptr)) {
                        map->enemy_spawn_points[j] = (Vector2) { .x=obj->x, .y=obj->y };
                        j++;
                    }
                }
            }
        }
        current_layer = current_layer->next;
    }
    Rectangle *recs = _alloc_map_collision_recs(map);
    create_collision_box_list(collision_boxes, recs, ENV_COLLISION_TYPE, map->collission_rec_list_size);
    _free_map_collision_recs(recs);

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
        }
        current_layer = current_layer->next;
    }

    return;
}

void destroy_tilemap(Tilemap *map) {
    for (int i=0; i<OBJECTS_GID_NUM; i++) {
        destroy_sprite(&map->obj_types[i].spr);
    }
    MemFree(map->enemy_spawn_points);
    MemFree(map->obj_blocks);
    destroy_sprite(&map->tileset);
    cute_tiled_free_map(map->tilemap);
    MemFree(map);

    return;
}

Rectangle *_alloc_map_collision_recs(Tilemap *map) {
    Rectangle *collision_recs = MemAlloc(sizeof(Rectangle) * (map->obj_blocks_size + map->invisible_recs_size));

    for (int i=0; i<map->obj_blocks_size; i++) {
        Rectangle collision_rec = map->obj_blocks[i].dest_rec;
        collision_rec.height /= 2;
        collision_rec.y +=collision_rec.height;
        collision_recs[i] = collision_rec;
    }
    for (int i=0; i<map->invisible_recs_size; i++) {
        collision_recs[map->obj_blocks_size+i] = map->invisible_recs[i];
    }

    return collision_recs;
}

void _free_map_collision_recs(Rectangle *collision_recs) {
    MemFree(collision_recs);

    return;
}

void _load_tiled_tilemap(Tilemap *map) {

    map->tilemap = cute_tiled_load_map_from_file("resources/data/maps/world.json", NULL);
    assert(map->tilemap);

    map->layer = map->tilemap->layers;
    map->tilemap_data = map->layer->data;
    map->tileset_data = map->tilemap->tilesets;
    map->cols = map->tileset_data->columns;

    init_sprite(&map->tileset, "resources/data/graphics/tilesets/world_tileset.png");
    map->tileset.src_rec = (Rectangle) {
        .x = 0, .y = 0,
        .width = map->tileset_data->tilewidth, .height = map->tileset_data->tileheight,
    };
    map->tileset.dest_rec = map->tileset.src_rec;

    return;
}

void _load_tiled_obj_sprites(Tilemap *map) {
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
        map->obj_types[i].gid = i + GRASSROCK1;
        map->obj_types[i].type = map_obj_type_list[i];
        init_sprite(&map->obj_types[i].spr, (char*)TextFormat("resources/data/graphics/objects/%s.png", map->obj_types[i].type));
    }

    return;
}

int _comp_y_value(const void * elem1, const void * elem2) {

    float f = ((MapObjBlock*)elem1)->dest_rec.y + ((MapObjBlock*)elem1)->dest_rec.height/2.f;
    float s = ((MapObjBlock*)elem2)->dest_rec.y + ((MapObjBlock*)elem2)->dest_rec.height/2.f;

    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}
