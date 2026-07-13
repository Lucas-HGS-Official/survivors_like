#include "Enemy.h"

#include <raymath.h>
#include <raylib.h>

#include "Sprite.h"
#include "settings.h"


#define FRAME_TIME .1f


void _update_animation(Enemy *enemy, float dt);
void _update_movement(Enemy *enemy, Vector2 player_position, float dt);

Enemy *init_enemy_types(void) {
    Enemy *enemy_types = (Enemy*)MemAlloc(sizeof(Enemy)*NUM_ENEMY_TYPES);

    for (ENEMY_TYPES i=0; i<NUM_ENEMY_TYPES; i++) {
        char *enemy_name = "";
        switch (i) {
            case BAT:
                enemy_name = "bat";
                break;
            case BLOB:
                enemy_name = "blob";
                break;
            case SKELETON:
                enemy_name = "skeleton";
                break;
            case NUM_ENEMY_TYPES:
                break;
        }
        for (int j=0; j<NUM_FRAMES; j++) {
            char *filepath = (char*) TextFormat("resources/images/enemies/%s/%i.png", enemy_name, j);
            init_sprite(&enemy_types[i].spr_anim[j], filepath);
        }
    }

    return enemy_types;
}
Enemy instance_enemy(Enemy *enemy, Vector2 spawn_point) {
    Enemy new_enemy = *enemy;
    new_enemy.is_visible = true;
    new_enemy.position = spawn_point;
    new_enemy.direction = (Vector2) { .x=1, .y=0 };
    new_enemy.speed = 200.f;
    new_enemy.frame_timer = FRAME_TIME;
    new_enemy.current_frame = 0;

    return new_enemy;
}
void update_enemy_list(Enemy *enemy_list, int enemy_list_size, Vector2 player_position, float dt) {
    for (int i=0; i<enemy_list_size; i++) {
        if (enemy_list[i].is_visible) {
            _update_animation(&enemy_list[i], dt);
            _update_movement(&enemy_list[i], player_position, dt);
        }
    }

    return;
}
void draw_enemy_list(Enemy *enemy_list, int enemy_list_size) {
    for (int i=0; i<enemy_list_size; i++) {
        if (enemy_list[i].is_visible) {
            draw_sprite(&enemy_list[i].spr_anim[enemy_list[i].current_frame], WHITE);
            // draw_sprite(&enemy_list[i].spr_anim[0], WHITE);
            // draw_sprite(&enemy_list[i].spr_anim[1], WHITE);
            // draw_sprite(&enemy_list[i].spr_anim[2], WHITE);
            // draw_sprite(&enemy_list[i].spr_anim[3], WHITE);
        }
    }

    return;
}
void destroy_enemy_types(Enemy *enemy_types) {
    for (ENEMY_TYPES i=0; i<NUM_ENEMY_TYPES; i++) {
        for (int j=0; j<NUM_FRAMES; j++) {
            destroy_sprite(&enemy_types[i].spr_anim[j]);
        }
    }
    MemFree(enemy_types);

    return;
}

void _update_animation(Enemy *enemy, float dt) {
    if (enemy->frame_timer <=0) {
        enemy->frame_timer = FRAME_TIME;
        enemy->current_frame++;
        if (enemy->current_frame >= NUM_FRAMES) {
            enemy->current_frame = 0;
        }
    }
    enemy->frame_timer -= dt;

    return;
}
void _update_movement(Enemy *enemy, Vector2 player_position, float dt) {
    Sprite *current_sprite = &enemy->spr_anim[enemy->current_frame];

    Vector2 position = enemy->position;

    enemy->direction = Vector2Normalize(Vector2Subtract(player_position, position));

    // update position
    position = Vector2Add(position, Vector2Scale(enemy->direction, enemy->speed * dt));
    enemy->position = position;

    current_sprite->dest_rec.x = enemy->position.x;
    current_sprite->dest_rec.y = enemy->position.y;

    return;
}
