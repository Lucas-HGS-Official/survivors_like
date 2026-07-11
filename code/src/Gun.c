#include "Gun.h"

#include <raylib.h>
#include <raymath.h>

#include "Player.h"
#include "Sprite.h"
#include "settings.h"


Gun *init_gun(Player *player) {
    Gun *gun = (Gun*)MemAlloc(sizeof(Gun));

    Sprite *current_player_sprite = &player->spr[player->facing_direction][player->current_frame];
    Vector2 player_center = {
        .x = current_player_sprite->dest_rec.x + current_player_sprite->dest_rec.width/2.f,
        .y = current_player_sprite->dest_rec.y + current_player_sprite->dest_rec.height/2.f,
    };

    gun->distance = 140;
    gun->direction = (Vector2){ .x=1, .y=0 };


    init_sprite(&gun->spr, "resources/images/gun/gun.png");
    gun->spr.dest_rec.x = player_center.x + Vector2Scale(gun->direction, gun->distance/2.f).x;
    gun->spr.dest_rec.y = player_center.y + Vector2Scale(gun->direction, gun->distance/2.f).y;
    gun->spr.origin = (Vector2) { .x=gun->spr.dest_rec.width/2.f, .y=gun->spr.dest_rec.height/2.f };

    gun->src_rec = gun->spr.src_rec;
    gun->src_rec_flip = gun->src_rec;
    gun->src_rec_flip.height *= -1;

    gun->tip = (Vector2) {
        .x=player_center.x + Vector2Scale(gun->direction, gun->distance + gun->spr.dest_rec.width/2.f).x,
        .y=player_center.y + Vector2Scale(gun->direction, gun->distance + gun->spr.dest_rec.height/2.f).y,
    };

    return gun;
}
void update_gun(Gun *gun, Player *player) {
    Sprite *current_player_sprite = &player->spr[player->facing_direction][player->current_frame];
    Vector2 player_center = {
        .x = current_player_sprite->dest_rec.x + current_player_sprite->dest_rec.width/2.f,
        .y = current_player_sprite->dest_rec.y + current_player_sprite->dest_rec.height/2.f,
    };
    Vector2 window_center = { .x=WINDOW_WIDTH/2.f, .y=WINDOW_HEIGHT/2.f };

    gun->direction = Vector2Normalize(Vector2Subtract(GetMousePosition(), window_center));
    gun->spr.dest_rec.x = player_center.x + Vector2Scale(gun->direction, gun->distance).x;
    gun->spr.dest_rec.y = player_center.y + Vector2Scale(gun->direction, gun->distance).y;
    gun->spr.rotation = Vector2Angle((Vector2) { .x=1 }, gun->direction)*57.5;
    if (gun->spr.rotation > 90.f || gun->spr.rotation < -90.f) {
        gun->spr.src_rec = gun->src_rec_flip;
    } else {
        gun->spr.src_rec = gun->src_rec;
    }

    gun->tip = (Vector2) {
        .x=player_center.x + Vector2Scale(gun->direction, gun->distance + gun->spr.dest_rec.width/2.f).x,
        .y=player_center.y + Vector2Scale(gun->direction, gun->distance + gun->spr.dest_rec.height/2.f).y,
    };

    return;
}
void draw_gun(Gun *gun) {
    draw_sprite(&gun->spr, WHITE);

    return;
}
void destroy_gun(Gun *gun) {
    destroy_sprite(&gun->spr);
    MemFree(gun);

    return;
}
