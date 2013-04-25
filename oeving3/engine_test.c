#include "engine.h"
#include "components.h"
#include "font.h"
#include "graphics.h"
#include "engine_test.h"

#include <stdio.h>
#include <stdlib.h>

//defines
void init();
void dispose();
void death_print(gameobject *object);
void ufo_offscreen(gameobject *object);
void player_death(gameobject *object);
void enemy_spawner();
void powerup_spawner();

drawable *rabby, *rabby_red, *bullet;
drawable *ufo1, *ufo2, *ufo3, *ufo4, *ufo5;
drawable *power_sprite[4];
gameobject *player1, *player2;


font *f_small, *f_large;

int main() {
    // Init components
    components_init();
    
    // Load fonts
    f_small = font_load("font_small");
    f_large = font_load("font_large");
    
    // Load images
    bmp_image *img_ufo1 = bmp_load("images/ufo1.bmp");
    bmp_image *img_ufo2 = bmp_load("images/ufo2.bmp");
    bmp_image *img_ufo3 = bmp_load("images/ufo3.bmp");
    bmp_image *img_ufo4 = bmp_load("images/ufo4.bmp");
    bmp_image *img_ufo5 = bmp_load("images/ufo5.bmp");
    bmp_image *img_rabby = bmp_load("rabbit.bmp");
    bmp_image *img_powerup1 = bmp_load("images/pow1.bmp");
    bmp_image *img_powerup2 = bmp_load("images/pow1.bmp");
    bmp_image *img_powerup3 = bmp_load("images/pow1.bmp");
    bmp_image *img_powerup4 = bmp_load("images/pow1.bmp");
    bmp_image *img_rabby_red = bmp_copy(img_rabby);
    bmp_tint(img_rabby_red, 255, 128, 128);
    bmp_tint(img_powerup1, 0, 214, 29);
    bmp_tint(img_powerup2, 0, 214, 150);
    bmp_tint(img_powerup3, 150, 214, 29);
    bmp_tint(img_powerup4, 255, 0, 29);
    
    // Create drawables
    ufo1 = drawable_create_bmp(img_ufo1);
    ufo2 = drawable_create_bmp(img_ufo2);
    ufo3 = drawable_create_bmp(img_ufo3);
    ufo4 = drawable_create_bmp(img_ufo4);
    ufo5 = drawable_create_bmp(img_ufo5);
    rabby = drawable_create_bmp(img_rabby);
    bullet = drawable_create_rect(4, 4, 255, 255, 255);
    rabby_red = drawable_create_bmp(img_rabby_red);
    power_sprite[0] = drawable_create_bmp(img_powerup1);
    power_sprite[1] = drawable_create_bmp(img_powerup2);
    power_sprite[2] = drawable_create_bmp(img_powerup3);
    power_sprite[3] = drawable_create_bmp(img_powerup4);
    
    // Loop game
    while (1) {
        engine_init();
        init();
        engine_run();
        dispose();
        engine_dispose();
    }
}

void init() {
    // Add object
    player1 = gameobject_create();
    player1->size_x = 32;
    player1->size_y = 32;
    player1->pos_x = SCREEN_WIDTH/2 + 10;
    player1->pos_y = 200;
    player1->size_x = 32;
    player1->size_y = 32;
    player1->type = TYPE_PLAYER1;
    player1->hp = player1->size_x;
    component_add(player1, component_player_control, (void*)0);
    component_add(player1, component_sprite, rabby);
    component_add(player1, component_shoot, bullet);
    component_add(player1, component_hpbar,(int[]) {player1->hp, 4}) ;
    component_add(player1, component_death, &player_death);
    engine_gameobject_add(player1);
    
    // Add object
    player2 = gameobject_create();
    player2->size_x = 32;
    player2->size_y = 32;
    player2->pos_x = SCREEN_WIDTH/2 - player2->size_x - 10;
    player2->pos_y = 200;
    player2->type = TYPE_PLAYER2;
    player2->size_x = 32;
    player2->size_y = 32;
    player2->hp = player2->size_x;
    component_add(player2, component_player_control, (void*)1);
    component_add(player2, component_sprite, rabby);
    component_add(player2, component_shoot, bullet);
    component_add(player2, component_hpbar, (int[]) {player2->hp, 4}) ;
    component_add(player2, component_death, &player_death);
    engine_gameobject_add(player2);
    
    //Add enemy spawner
    engine_ticker_add(&enemy_spawner);
    engine_ticker_add(&powerup_spawner);
}


void dispose() {
    return;
}


void enemy_spawner(){
    //Spawn enemy every 100th tick 
    if(TICK % 100 == 0){
     // Add object
        gameobject *enemy = gameobject_create();
        drawable *sprite = NULL;
        
        // Randomize
        int r = rand() % 5 + 1;
        if (r == 1) {
            sprite = ufo1;
            enemy->size_x = 60;
            enemy->size_y = 30;
        }
        else if (r == 2) {
            sprite = ufo2;
            enemy->size_x = 30;
            enemy->size_y = 15;
        }
        else if (r == 3) {
            sprite = ufo3;
            enemy->size_x = 30;
            enemy->size_y = 15;
        }
        else if (r == 4) {
            sprite = ufo4;
            enemy->size_x = 30;
            enemy->size_y = 15;
        }
        else if (r == 5) {
            sprite = ufo5;
            enemy->size_x = 30;
            enemy->size_y = 15;
        }
        
        
        enemy->type = TYPE_ENEMY;
        enemy->pos_x = rand() % (SCREEN_WIDTH - enemy->size_x - 40) + 20;
        enemy->pos_y = - (enemy->size_y - 1);
        enemy->hp = enemy->size_x;
        
        // Add collision effect
        component_collision_data data = {
            .target_type = TYPE_PLAYER1 | TYPE_PLAYER2,
            .self_effect = component_gameobject_remove,
            .self_param = NULL,
            .other_effect = component_damage,
            .other_param = (void*)10,
        };
        component_add(enemy, component_collision, &data);
        component_add(enemy, component_sprite, sprite);
        component_add(enemy, component_hpbar, (int[]) {enemy->hp, 4}) ;
        component_add(enemy, component_zigzag, &(component_zigzag_data){2, 50});
        component_add(enemy, component_move, &(component_move_data){0, 1});
        component_add(enemy, component_offscreen, &ufo_offscreen);
        component_add(enemy, component_death, &death_print);
        
        engine_gameobject_add(enemy);
    }
}

void powerup_spawner(){
    if(TICK % 400 == 50){
     // Add object
        gameobject *powerup = gameobject_create();
        powerup->type = TYPE_NONE;
        powerup->size_x = 30;
        powerup->size_y = 20;
        powerup->pos_x = rand() % (SCREEN_WIDTH - 60) + 30;
        powerup->pos_y = -10;
        
        // Use random image
        drawable *sprite;

        int r = rand() % 4;
        sprite = power_sprite[r];
        int led_nr = r;
        component *self_effect = component_damage;
        void* self_param = 0;
        component *enemy_effect = component_damage;
        void* enemy_param = 0;
        switch(r){
            case 0:
                self_param = (void*) -10;
                enemy_param = (void*) 0;
                break;
            case 1:
                self_param = (void*) -7;
                enemy_param = (void*) 3;
                break;
            case 2:
                self_param = (void*) -5;
                enemy_param = (void*) 5;
                break;
            case 3:
                self_param = (void*) 0;
                enemy_param = (void*) 10;
                break;
        };
        component_powerup_data *data1 = malloc(sizeof(component_powerup_data));

        *data1 = (component_powerup_data){
            .led_nr = led_nr,
                .self_effect = self_effect,
                .self_param = self_param,
                .enemy_effect = enemy_effect,
                .enemy_param = enemy_param,
        };
        //TODO: Free where?

        // Add collision effect
        component_collision_data data2 = {
            .target_type = TYPE_PLAYER1 | TYPE_PLAYER2,
            .self_effect = component_gameobject_remove,
            .self_param = NULL,
            .other_effect = component_powerup,
            .other_param = data1,
        };

        component_add(powerup, component_collision, &data2);
        component_add(powerup, component_sprite, sprite);
        component_add(powerup, component_move, &(component_move_data){0, 2});
        engine_gameobject_add(powerup);
    }
}

void death_print(gameobject *object){
}

void ufo_offscreen(gameobject *object) {
    component_add(player1, component_damage, (void*)5);
    component_add(player2, component_damage, (void*)5);
    engine_gameobject_remove(object);
}

void player_death(gameobject *object) {
    // Check for repeat
    if (player1->hp == player2->hp && object == player2) {
        return;
    }
    
    screen_fill(255,0,0);
    if (player1->hp == player2->hp) {
        screen_draw_text(40, 100, f_large, "ALL RABBITS DIE!");
        screen_draw_text(40, 130, f_small, "Aliens have scorched the land...");
        screen_draw_text(40, 150, f_small, "Pixie-land is no more...");
    }
    else if (object == player1) {
        screen_draw_text(40, 100, f_large, "RABBIT 1 DIES!");
        screen_draw_text(40, 130, f_small, "While rabbit 2 lives on...");
        screen_draw_text(40, 150, f_small, "For now...");
    } else {
        screen_draw_text(40, 100, f_large, "RABBIT 2 DIES!");
        screen_draw_text(40, 130, f_small, "While rabbit 1 lives on...");
        screen_draw_text(40, 150, f_small, "For now...");
    }
    screen_update_all();
    
    ENGINE_RUNNING = 0;
    sleep(3);
}
