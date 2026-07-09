#include "Sprite.h"

#include <raylib.h>
#include <stdlib.h>

#include "settings.h"


static int _comp_y_value(const void * elem1, const void * elem2);

void init_sprite(Sprite *sprite, char *texture_file_path) {
    // Initiating all necessary values to draw the a Texture2D
    sprite->texture = MemAlloc(sizeof(Texture2D));
    *sprite->texture = LoadTexture(texture_file_path);
    sprite->src_rec = (Rectangle) {
        .width = sprite->texture->width, .height = sprite->texture->height,
        .x = 0, .y = 0,
    };
    sprite->dest_rec = (Rectangle) {
        .height = sprite->src_rec.height, .width = sprite->src_rec.width,
        .x = WINDOW_WIDTH/2.f, .y = WINDOW_HEIGHT/2.f,
    };
    sprite->origin = (Vector2) {0};
    sprite->rotation = 0.f;
    sprite->tint = WHITE;

    return;
}
void draw_sprite(Sprite *sprite, Color sprite_tint) {
    DrawTexturePro(*sprite->texture, sprite->src_rec, sprite->dest_rec, sprite->origin, sprite->rotation, sprite_tint);

    return;
}
void sort_and_draw_sprite_list(Sprite *sprite_list, int sprite_list_size) {

    qsort(sprite_list, sprite_list_size, sizeof(Sprite), _comp_y_value);

    for (int i=0; i<sprite_list_size; i++) {
        draw_sprite(&sprite_list[i], WHITE);
    }

    return;
}
void destroy_sprite(Sprite *sprite) {
    UnloadTexture(*sprite->texture);
    MemFree(sprite->texture);

    return;
}

int _comp_y_value(const void * elem1, const void * elem2) {

    float f = ((Sprite*)elem1)->dest_rec.y + ((Sprite*)elem1)->dest_rec.height/2.f;
    float s = ((Sprite*)elem2)->dest_rec.y + ((Sprite*)elem2)->dest_rec.height/2.f;

    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}
