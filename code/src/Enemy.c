#include "Enemy.h"

#include <raylib.h>

#include "Sprite.h"
#include "settings.h"


EnemyTypes *init_enemy_types(void){
    EnemyTypes *enemy_types = (EnemyTypes*)MemAlloc(sizeof(EnemyTypes));

    enemy_types->spr_list = (Sprite**) MemAlloc(sizeof(Sprite*) * NUM_ENEMY_TYPES);
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
        enemy_types->spr_list[i] = (Sprite*) MemAlloc(sizeof(Sprite) * NUM_FRAMES);
        for (int j=0; j<NUM_FRAMES; j++) {
            char *filepath = (char*) TextFormat("resources/images/enemies/%s/%i.png", enemy_name, j);
            init_sprite(&enemy_types->spr_list[i][j], filepath);
        }
    }

    return enemy_types;
}
Enemy instance_enemy(Sprite *enemy_sprites){
    Enemy enemy;
    enemy.spr_anim = enemy_sprites;
    enemy.direction = (Vector2) { .x=1, .y=0 };
    enemy.speed = 40.f;

    return enemy;
}
void update_enemy_list(Enemy *enemy_list){

    return;
}
void destroy_enemy_types(EnemyTypes *enemy_types){
    for (ENEMY_TYPES i=0; i<NUM_ENEMY_TYPES; i++) {
        for (int j=0; j<NUM_FRAMES; j++) {
            destroy_sprite(&enemy_types->spr_list[i][j]);
        }
    }
    MemFree(enemy_types->spr_list);
    MemFree(enemy_types);

    return;
}
