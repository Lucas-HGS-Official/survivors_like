#include "Player.h"

#include <raymath.h>

#include "settings.h"
#include "Sprite.h"

void init_player(Player *player) {
    init_sprite(&(player->spr), "resources/images/player/down/0.png");
    player->direction = (Vector2) {};
    player->direction = Vector2Normalize(player->direction);
    player->speed = 400.f;

    return;
}
void update_player(Player *player,float dt) {
    player->direction = (Vector2){
        .x = (int)IsKeyDown(KEY_RIGHT) - (int)IsKeyDown(KEY_LEFT),
        .y =  IsKeyDown(KEY_DOWN) - (int)IsKeyDown(KEY_UP)
    };
    player->direction = Vector2Normalize(player->direction);

    Vector2 position = {
        .x = player->spr.dest_rec.x + (player->direction.x * player->speed * dt),
        .y = player->spr.dest_rec.y + (player->direction.y * player->speed * dt),
    };
    Vector2 half_size = {
        .x = player->spr.dest_rec.width/2.f,
        .y = player->spr.dest_rec.height/2.f,
    };
    player->spr.dest_rec.x = Clamp(position.x, half_size.x, WINDOW_WIDTH - half_size.x);
    player->spr.dest_rec.y = Clamp(position.y, half_size.y, WINDOW_HEIGHT - half_size.y);


    return;
}
void destroy_player(Player* player) {
    destroy_sprite(&player->spr);

    return;
}
