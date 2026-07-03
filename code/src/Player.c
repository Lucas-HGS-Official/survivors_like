#include "Player.h"

#include <raylib.h>
#include <raymath.h>

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
    Rectangle hitbox_rec;

    PlayerFacingDir facing_direction;
    PlayerAnimState anim_state;
    float frame_timer;
    float current_frame_time;
    int current_frame;
    Color tint;
} Player;

void _start_animation(Player *player);
void _stop_animation(Player *player);

void _controls(Player *player);
void _movement(Player *player, float dt);

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
    player->current_frame_time = 0.1f;
    player->frame_timer = 0;
    player->tint = WHITE;

    player->direction = (Vector2) {};
    player->speed = 400.f;
    player->position = (Vector2) {
        .x = WINDOW_WIDTH/2.f,
        .y = WINDOW_HEIGHT/2.f,
    };

    Sprite *current_sprite = &player->spr[player->facing_direction][player->current_frame];
    Rectangle player_hitbox_rec = current_sprite->dest_rec;
    player_hitbox_rec.x -= player_hitbox_rec.width/2.f;
    player_hitbox_rec.y -= player_hitbox_rec.height/2.f;
    player->hitbox_rec = player_hitbox_rec;

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
    _controls(player);

    // Animation state machine
    switch (player->anim_state) {
        case STOPPED_PLAYER:
            player->current_frame = 0;
            player->frame_timer = 0;
            break;
        case WALKING_PLAYER:
            if (player->frame_timer <= GetTime() - player->current_frame_time) {
                player->current_frame += 1;
                if (player->current_frame >= NUM_FRAMES) {
                    player->current_frame = 0;
                }
                player->frame_timer = GetTime();
            }
            break;
        case NUM_ANIM_STATES:
            break;
    }

    _movement(player, dt);

    return;
}
void draw_player(Player *player) {
    Sprite *current_sprite = &player->spr[player->facing_direction][player->current_frame];
    draw_sprite(current_sprite, player->tint);

    // Draw test collision recs (delete later)
    DrawRectangleRec(collision_test_rec_1, RED);
    DrawRectangleRec(collision_test_rec_2, GREEN);
    DrawRectangleRec(collision_test_rec_3, BLUE);

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

void player_collision(Player *player, char collision_mode, Rectangle* hitbox_rec_list, int num_recs) {
    Rectangle player_hitbox_rec = player->hitbox_rec;
    player_hitbox_rec.x -= player_hitbox_rec.width/2.f;
    player_hitbox_rec.y -= player_hitbox_rec.height/2.f;
    for (int i=0; i<num_recs; i++) {
        if (CheckCollisionRecs(player_hitbox_rec, hitbox_rec_list[i])) {

            float collided_right_side = hitbox_rec_list[i].x + hitbox_rec_list[i].width;
            float collided_left_left = hitbox_rec_list[i].x;
            float collided_top_side = hitbox_rec_list[i].y;
            float collided_bottom_side = hitbox_rec_list[i].y + hitbox_rec_list[i].height;


            if (collision_mode == 'h') {
                if (player->direction.x > 0) {
                    player->hitbox_rec.x = collided_left_left - player_hitbox_rec.width/2.f;
                }
                if (player->direction.x < 0) {
                    player->hitbox_rec.x = collided_right_side + player_hitbox_rec.width/2.f;
                }
            } else {
                if (player->direction.y > 0) {
                    player->hitbox_rec.y = collided_top_side - player_hitbox_rec.height/2.f;
                }
                if (player->direction.y < 0) {
                    player->hitbox_rec.y = collided_bottom_side + player_hitbox_rec.height/2.f;
                }
            }
        }
    }
}

void _start_animation(Player *player) {
    player->anim_state = WALKING_PLAYER;
    if (player->frame_timer == 0) { player->frame_timer = GetTime(); }

    return;
}
void _stop_animation(Player *player) {
    player->anim_state = STOPPED_PLAYER;
    player->frame_timer = 0;

    return;
}

void _controls(Player *player) {
    player->direction = (Vector2){
        .x = (float)IsKeyDown(KEY_RIGHT) - (float)IsKeyDown(KEY_LEFT),
        .y = (float)IsKeyDown(KEY_DOWN) - (float)IsKeyDown(KEY_UP)
    };
    player->direction = Vector2Normalize(player->direction);

    if (!IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) { player->direction.y = 0; }
    if (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) { player->direction.x = 0; }

    player->direction = Vector2Normalize(player->direction);

    if (player->direction.x > 0) { player->facing_direction = RIGHT_FACE_PLAYER; }
    if (player->direction.x < 0) { player->facing_direction = LEFT_FACE_PLAYER; }
    if (player->direction.y > 0) { player->facing_direction = DOWN_FACE_PLAYER; }
    if (player->direction.y < 0) { player->facing_direction = UP_FACE_PLAYER; }

    if (player->direction.y == 0 && player->direction.x == 0) {
        _stop_animation(player);
    } else { _start_animation(player); }


    return;
}

void _movement(Player *player, float dt) {
    // Player movement
    Sprite *current_sprite = &player->spr[player->facing_direction][player->current_frame];

    player->hitbox_rec.x += player->direction.x * player->speed * dt;
    player_collision(player, HORIZONTAL_COLLISION_MODE, collision_list, 3);
    player->hitbox_rec.y += player->direction.y * player->speed * dt;
    player_collision(player, VERTICAL_COLLISION_MODE, collision_list, 3);

    Vector2 half_size = {
        .x = player->hitbox_rec.width/2.f,
        .y = player->hitbox_rec.height/2.f,
    };
    player->hitbox_rec.x = Clamp(player->hitbox_rec.x, half_size.x, WINDOW_WIDTH - half_size.x);
    player->hitbox_rec.y = Clamp(player->hitbox_rec.y, half_size.y, WINDOW_HEIGHT - half_size.y);

    player->position = (Vector2) {
        .x = player->hitbox_rec.x,
        .y = player->hitbox_rec.y,
    };

    current_sprite->dest_rec.x = player->position.x;
    current_sprite->dest_rec.y = player->position.y;

    return;
}
