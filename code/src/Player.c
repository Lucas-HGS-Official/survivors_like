#include "Player.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "settings.h"
#include "Sprite.h"

// All player animations have for 4 frames
#define NUM_FRAMES 4

#define HORIZONTAL_COLLISION_MODE 'h'
#define VERTICAL_COLLISION_MODE 'v'


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
    Rectangle collision_rec;

    PlayerFacingDir facing_direction;
    PlayerAnimState anim_state;
    int current_frame;
    Color tint;
} Player;

Rectangle collision_test_rec_1 = {0};
Rectangle collision_test_rec_2 = {0};
Rectangle collision_test_rec_3 = {0};
Rectangle collision_list[3] = {0};


Player *init_player(void) {
    Player *player = (Player*) MemAlloc(sizeof(Player));

    // Loading all images for each player frame
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

    // Initiating all other player values
    player->facing_direction = DOWN_FACE_PLAYER;
    player->anim_state = WALKING_PLAYER;
    player->current_frame = 0;
    player->tint = WHITE;

    player->direction = (Vector2) {};
    player->speed = 400.f;
    player->position = (Vector2) {
        .x = WINDOW_WIDTH/2.f,
        .y = WINDOW_HEIGHT/2.f,
    };

    // Initiating test collision rectangles (Delete latter)
    collision_test_rec_1 = (Rectangle) {
        .height = 200,
        .width = 200,
    };

    collision_test_rec_2 = collision_test_rec_1;
    collision_test_rec_2.x = (WINDOW_WIDTH/2.f) + 100;
    collision_test_rec_2.y = player->spr[0]->texture[0].height + 20;

    collision_test_rec_3 = (Rectangle) {
        .height = 150,
        .width = 250,
        .x = WINDOW_WIDTH/2.f,
        .y = WINDOW_HEIGHT - (player->spr[0]->texture[0].height + 20),
    };

    collision_list[0] = collision_test_rec_1;
    collision_list[1] = collision_test_rec_2;
    collision_list[2] = collision_test_rec_3;

    return player;
}
void update_player(Player *player,float dt) {
    // Player controls
    if (IsKeyDown(KEY_RIGHT)) {
        player->direction.x = 1;
        player->facing_direction = RIGHT_FACE_PLAYER;
        player_collision_rec(player, HORIZONTAL_COLLISION_MODE, collision_list, 3);
    }
    if (IsKeyDown(KEY_LEFT)) {
        player->direction.x = -1;
        player->facing_direction = LEFT_FACE_PLAYER;
        player_collision_rec(player, HORIZONTAL_COLLISION_MODE, collision_list, 3);
    }
    if (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) { player->direction.x = 0; }

    if (IsKeyDown(KEY_DOWN)) {
        player->direction.y = 1;
        player->facing_direction = DOWN_FACE_PLAYER;
        player_collision_rec(player, VERTICAL_COLLISION_MODE, collision_list, 3);
    }
    if (IsKeyDown(KEY_UP)) {
        player->direction.y = -1;
        player->facing_direction = UP_FACE_PLAYER;
        player_collision_rec(player, VERTICAL_COLLISION_MODE, collision_list, 3);
    }
    if (!IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) { player->direction.y = 0; }

    // TODO: chance animation state
    // TODO: animation state machine

    // Player movement
    player->direction = Vector2Normalize(player->direction);

    Sprite *current_sprite = &player->spr[player->facing_direction][player->current_frame];

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

    Rectangle player_collision_rec = current_sprite->dest_rec;
    player_collision_rec.x -= player_collision_rec.width/2.f;
    player_collision_rec.y -= player_collision_rec.height/2.f;

    player->collision_rec = player_collision_rec;

    return;
}
void draw_player(Player *player) {
    Sprite *current_sprite = &player->spr[player->facing_direction][player->current_frame];
    current_sprite->dest_rec.x = player->position.x;
    current_sprite->dest_rec.y = player->position.y;
    draw_sprite(current_sprite, player->tint);

    // Draw test collision recs (delete later)
    DrawRectangleRec(collision_test_rec_1, RED);
    DrawRectangleRec(collision_test_rec_2, GREEN);
    DrawRectangleRec(collision_test_rec_3, BLUE);

    // DrawRectangleLinesEx(player->collision_rec, 5, YELLOW);

    return;
}
void destroy_player(Player* player) {
    // Unloading each texture
    for (int i=0; i<NUM_FACE_PLAYER; i++) {
        for (int j=0; j<NUM_FRAMES; j++) {
            destroy_sprite(&player->spr[i][j]);
        }
    }
    MemFree(player->spr);
    MemFree(player);

    return;
}

void player_collision_rec(Player *player, char collision_mode, Rectangle* collision_rec_list, int num_recs) {
    for (int i=0; i<num_recs; i++) {
        if (CheckCollisionRecs(player->collision_rec, collision_rec_list[i])) {
            printf("\n Overlap in %c mode\n", collision_mode);
        }
    }
}
