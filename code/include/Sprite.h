#pragma once

#include <raylib.h>

typedef struct Sprite {
    Texture2D *texture;
    Rectangle src_rec;
    Rectangle dest_rec;
    Vector2 origin;
    float rotation;
    Color tint;
} Sprite;

void init_sprite(Sprite *sprite, char *texture_file_path);
void draw_sprite(Sprite *sprite, Color sprite_tint);
void sort_and_draw_sprite_list(Sprite *sprite_list, int sprite_list_size);
void destroy_sprite(Sprite *sprite);
