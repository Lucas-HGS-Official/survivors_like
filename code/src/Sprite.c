#include "Sprite.h"

#include "settings.h"
#include <raylib.h>


void init_sprite(Sprite *sprite, char *texture_file_path) {
    sprite->texture = MemAlloc(sizeof(Texture2D));
    *(sprite->texture) = LoadTexture(texture_file_path);
    sprite->src_rec = (Rectangle) {
        .width = sprite->texture->width, .height = sprite->texture->height,
        .x = 0, .y = 0,
    };
    sprite->dest_rec = (Rectangle) {
        .height = sprite->src_rec.height, .width = sprite->src_rec.width,
        .x = WINDOW_WIDTH/2.f, .y = WINDOW_HEIGHT/2.f,
    };
    sprite->origin = (Vector2) { .x = sprite->src_rec.width/2.f, .y = sprite->src_rec.height/2.f };
    sprite->rotation = 0.f;
    sprite->tint = WHITE;

    return;
}
void draw_sprite(Sprite *sprite, Color sprite_tint) {
    DrawTexturePro(*(sprite->texture), sprite->src_rec, sprite->dest_rec, sprite->origin, sprite->rotation, sprite_tint);

    return;
}
void destroy_sprite(Sprite *sprite) {
    UnloadTexture(*(sprite->texture));
    MemFree(sprite->texture);

    return;
}
