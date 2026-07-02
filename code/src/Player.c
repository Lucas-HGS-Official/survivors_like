#include "Player.h"

#include <raylib.h>
#include <raymath.h>

#include "settings.h"
#include "Sprite.h"


#define NUM_FRAMES 4

typedef enum PlayerAnimState {
    STOPPED_PLAYER,
    WALKING_PLAYER,

    NUM_ANIM_STATES,
} PlayerAnimState;

typedef enum PlayerFacingDir {
    DOWN_FACE_PLAYER,
    UP_FACE_PLAYER,
    RIGHT_FACE_PLAYER,
    LEFT_FACE_PLAYER,

    NUM_FACE_PLAYER,
} PlayerFacingDir;

typedef struct Player {
    Sprite **spr;
    Vector2 position;
    Vector2 direction;
    float speed;

    PlayerFacingDir facing_dir;
    PlayerAnimState anim_state;
    int current_frame;
    Color tint;
} Player;


Player *init_player(void) {
    Player *player = (Player*) MemAlloc(sizeof(Player));

    player->spr = (Sprite**) MemAlloc(sizeof(Sprite*)*NUM_FACE_PLAYER);
    for (int i=0; i<NUM_FACE_PLAYER; i++) {
        char *facing_dir = "";
        switch (i) {
            case DOWN_FACE_PLAYER:
                facing_dir = "down";
                break;
            case UP_FACE_PLAYER:
                facing_dir = "up";
                break;
            case RIGHT_FACE_PLAYER:
                facing_dir = "right";
                break;
            case LEFT_FACE_PLAYER:
                facing_dir = "left";
                break;
        }
        player->spr[i] = (Sprite*) MemAlloc(sizeof(Sprite)*NUM_FRAMES);
        for (int j=0; j<NUM_FRAMES; j++) {
            char *filepath = (char*) TextFormat("resources/images/player/%s/%i.png", facing_dir, j);
            init_sprite(&(player->spr[i][j]), filepath);
        }
    }

    player->facing_dir = UP_FACE_PLAYER;
    player->anim_state = STOPPED_PLAYER;
    player->current_frame = 0;
    player->tint = WHITE;

    player->direction = (Vector2) {};
    player->speed = 400.f;
    player->position = (Vector2) {
        .x = WINDOW_WIDTH/2.f,
        .y = WINDOW_HEIGHT/2.f,
    };

    return player;
}
void update_player(Player *player,float dt) {
    if (IsKeyDown(KEY_RIGHT)) {
        player->direction.x = 1;
        player->facing_dir = RIGHT_FACE_PLAYER;
    }
    if (IsKeyDown(KEY_LEFT)) {
        player->direction.x = -1;
        player->facing_dir = LEFT_FACE_PLAYER;
    }
    if (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) { player->direction.x = 0; }

    if (IsKeyDown(KEY_DOWN)) {
        player->direction.y = 1;
        player->facing_dir = DOWN_FACE_PLAYER;
    }
    if (IsKeyDown(KEY_UP)) {
        player->direction.y = -1;
        player->facing_dir = UP_FACE_PLAYER;
    }
    if (!IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) { player->direction.y = 0; }
    player->direction = Vector2Normalize(player->direction);

    Sprite *current_sprite = &player->spr[player->facing_dir][player->current_frame];

    player->position = (Vector2) {
        .x = player->position.x + (player->direction.x * player->speed * dt),
        .y = player->position.y + (player->direction.y * player->speed * dt),
    };
    Vector2 half_size = {
        .x = current_sprite->dest_rec.width/2.f,
        .y = current_sprite->dest_rec.height/2.f,
    };
    player->position.x = Clamp(player->position.x, half_size.x, WINDOW_WIDTH - half_size.x);
    player->position.y = Clamp(player->position.y, half_size.y, WINDOW_HEIGHT - half_size.y);

    return;
}
void draw_player(Player *player) {
    Sprite *current_sprite = &player->spr[player->facing_dir][player->current_frame];
    current_sprite->dest_rec.x = player->position.x;
    current_sprite->dest_rec.y = player->position.y;
    draw_sprite(current_sprite, player->tint);

    return;
}
void destroy_player(Player* player) {
    for (int i=0; i<NUM_FACE_PLAYER; i++) {
        for (int j=0; j<NUM_FRAMES; j++) {
            destroy_sprite(&player->spr[i][j]);
        }
    }
    MemFree(player->spr);
    MemFree(player);

    return;
}
