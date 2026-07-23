#include "Enemy.h"

#include <raymath.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

#include "CollisionBoxes.h"
#include "Sprite.h"
#include "settings.h"


#define FRAME_TIME .1f

#define HORIZONTAL_ENEMY_SPR_PADDING 30
#define VERTICAL_ENEMY_SPR_PADDING 20

#define HORIZONTAL_COLLISION_MODE 'h'
#define VERTICAL_COLLISION_MODE 'v'


static void _update_animation(Enemy *enemy, float dt);
static void _update_movement(Enemy *enemy, Vector2 player_position, CollisionBoxList *collision_boxes, float dt);
void _update_collision(Enemy *enemy, char collision_mode, CollisionBoxList *collision_rec_list);


Enemy *init_enemy_types(CollisionBoxList *collision_boxes) {
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

    collision_boxes[ENEMY_COLLISION_TYPE].type = ENEMY_COLLISION_TYPE;
    collision_boxes[ENEMY_COLLISION_TYPE].size = MAX_NUM_ENEMIES;
    collision_boxes[ENEMY_COLLISION_TYPE].list = (Rectangle*)MemAlloc(sizeof(Rectangle) * MAX_NUM_ENEMIES);
    for (int i=0; i<MAX_NUM_ENEMIES; i++) {
        collision_boxes[ENEMY_COLLISION_TYPE].list[i] = (Rectangle) { 0 };
    }

    return enemy_types;
}
Enemy instance_enemy(Enemy *enemy, Vector2 spawn_point) {
    Enemy new_enemy = *enemy;
    new_enemy.is_visible = true;
    new_enemy.is_marked_for_deletion = false;
    new_enemy.position = spawn_point;
    new_enemy.direction = (Vector2) { .x=1, .y=0 };
    new_enemy.speed = 200.f;
    new_enemy.frame_timer = FRAME_TIME;
    new_enemy.current_frame = 0;

    Rectangle enemy_hitbox_rec = new_enemy.spr_anim[0].dest_rec;
    enemy_hitbox_rec.x = new_enemy.position.x;
    enemy_hitbox_rec.y = new_enemy.position.y;
    enemy_hitbox_rec.x += HORIZONTAL_ENEMY_SPR_PADDING;
    enemy_hitbox_rec.width -= HORIZONTAL_ENEMY_SPR_PADDING*2.f;
    enemy_hitbox_rec.y += VERTICAL_ENEMY_SPR_PADDING;
    enemy_hitbox_rec.height -= VERTICAL_ENEMY_SPR_PADDING;

    new_enemy.hitbox_rec = enemy_hitbox_rec;

    return new_enemy;
}
void update_enemy_list(Enemy *enemy_list, int enemy_list_size, Vector2 player_position, CollisionBoxList *collision_boxes, float dt) {
    for (int i=0; i<enemy_list_size; i++) {
        collision_boxes[ENEMY_COLLISION_TYPE].list[i] = (Rectangle) {0};
        if (enemy_list[i].is_visible) {
            if (enemy_list[i].is_marked_for_deletion) {
                enemy_list[i].is_visible = false;
            }
            _update_animation(&enemy_list[i], dt);
            _update_movement(&enemy_list[i], player_position, collision_boxes, dt);
            collision_boxes[ENEMY_COLLISION_TYPE].list[i] = enemy_list[i].hitbox_rec;
        }
    }

    return;
}
void draw_enemy_list(Enemy *enemy_list, int enemy_list_size) {
    for (int i=0; i<enemy_list_size; i++) {
        if (enemy_list[i].is_visible) {
            draw_sprite(&enemy_list[i].spr_anim[enemy_list[i].current_frame], WHITE);
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
void _update_movement(Enemy *enemy, Vector2 player_position, CollisionBoxList *collision_boxes, float dt) {
    Sprite *current_sprite = &enemy->spr_anim[enemy->current_frame];

    enemy->direction = Vector2Normalize(Vector2Subtract(player_position, enemy->position));

    // update position
    enemy->hitbox_rec.x += enemy->direction.x * enemy->speed * dt;
    _update_collision(enemy, HORIZONTAL_COLLISION_MODE, collision_boxes);
    enemy->hitbox_rec.y += enemy->direction.y * enemy->speed * dt;
    _update_collision(enemy, VERTICAL_COLLISION_MODE, collision_boxes);

    enemy->position.x = enemy->hitbox_rec.x - HORIZONTAL_ENEMY_SPR_PADDING;
    enemy->position.y = enemy->hitbox_rec.y - VERTICAL_ENEMY_SPR_PADDING;

    current_sprite->dest_rec.x = enemy->position.x;
    current_sprite->dest_rec.y = enemy->position.y;

    return;
}
void _update_collision(Enemy *enemy, char collision_mode, CollisionBoxList *collision_boxes) {
    if (collision_boxes == NULL) {
        return;
    }

    CollisionBox enemy_box = {
        .rec = enemy->hitbox_rec,
        .type = PLAYER_COLLISION_TYPE,
    };
    CollisionBox colllided_box = check_collision_box_list(enemy_box, collision_boxes);

    if (colllided_box.type == BULLET_COLLISION_TYPE) {
        enemy->is_marked_for_deletion = true;
    }

    if (colllided_box.type == ENV_COLLISION_TYPE) {
        float collided_right_side = colllided_box.rec.x + colllided_box.rec.width;
        float collided_left_left = colllided_box.rec.x;
        float collided_top_side = colllided_box.rec.y;
        float collided_bottom_side = colllided_box.rec.y + colllided_box.rec.height;

        if (collision_mode == 'h') {
            if (enemy->direction.x > 0) {
                enemy->hitbox_rec.x = collided_left_left - enemy->hitbox_rec.width;
            }
            if (enemy->direction.x < 0) {
                enemy->hitbox_rec.x = collided_right_side;
            }
        } else {
            if (enemy->direction.y > 0) {
                enemy->hitbox_rec.y = collided_top_side - enemy->hitbox_rec.height;
            }
            if (enemy->direction.y < 0) {
                enemy->hitbox_rec.y = collided_bottom_side;
            }
        }
    }

    return;
}
