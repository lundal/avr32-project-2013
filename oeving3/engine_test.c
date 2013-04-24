#include "engine.h"
#include "components.h"
#include "font.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

//defines
void death_print(gameobject *object);
void ufo_offscreen(gameobject *object);
void player_death(gameobject *object);
void enemy_spawner();
void powerup_spawner();

drawable *rabby_red, *power_sprite;
drawable *ufo1, *ufo2, *ufo3, *ufo4, *ufo5;
gameobject *player1, *player2;

font *f_small, *f_large;

int main() {
    engine_init();
    
    // Init components
    components_init();
    
    // Load images
    bmp_image *img_ufo1 = bmp_load("images/ufo1.bmp");
    bmp_image *img_ufo2 = bmp_load("images/ufo2.bmp");
    bmp_image *img_ufo3 = bmp_load("images/ufo3.bmp");
    bmp_image *img_ufo4 = bmp_load("images/ufo4.bmp");
    bmp_image *img_ufo5 = bmp_load("images/ufo5.bmp");
    bmp_image *img_rabby = bmp_load("rabbit.bmp");
    bmp_image *img_powerup = bmp_load("images/pow1.bmp");
    bmp_image *img_rabby_red = bmp_copy(img_rabby);
    bmp_tint(img_rabby_red, 255, 128, 128);
    bmp_tint(img_powerup, 0, 214, 29);
    
    // Load fonts
    f_small = font_load("font_small");
    f_large = font_load("font_large");
    
    // Create drawables
    ufo1 = drawable_create_bmp(img_ufo1);
    ufo2 = drawable_create_bmp(img_ufo2);
    ufo3 = drawable_create_bmp(img_ufo3);
    ufo4 = drawable_create_bmp(img_ufo4);
    ufo5 = drawable_create_bmp(img_ufo5);
    drawable *rabby = drawable_create_bmp(img_rabby);
    drawable *bullet = drawable_create_rect(5, 5, 255,255,255);
    rabby_red = drawable_create_bmp(img_rabby_red);
    power_sprite = drawable_create_bmp(img_powerup);
    
    // Add object
    player1 = gameobject_create();
    player1->pos_y = 200;
    player1->size_x = 32;
    player1->size_y = 32;
    player1->type = TYPE_PLAYER1;
    component_add(player1, component_player_control, (void*)0);
    component_add(player1, component_sprite, rabby);
    component_add(player1, component_shoot, bullet);
    component_add(player1, component_hpbar,(int[]) {30,5}) ;
    component_add(player1, component_death, &player_death);
    player1->hp = 20;
    engine_gameobject_add(player1);
    
    // Add object
    player2 = gameobject_create();
    player2->pos_y = 200;
    player2->type = TYPE_PLAYER1;
    player2->size_x = 32;
    player2->size_y = 32;
    component_add(player2, component_player_control, (void*)1);
    component_add(player2, component_sprite, rabby);
    component_add(player2, component_shoot, bullet);
    component_add(player2, component_hpbar, (int[]) {30,5}) ;
    component_add(player2, component_death, &player_death);
    player2->hp = 20;
    engine_gameobject_add(player2);
    
    //Add enemy spawner
    engine_ticker_add(&enemy_spawner);
    engine_ticker_add(&powerup_spawner);

    engine_run();
    
    return 0;
}


void enemy_spawner(){
    //Spawn enemy every 100th tick 
    if(TICK % 100 == 0){
     // Add object
        gameobject *enemy = gameobject_create();
        enemy->type = TYPE_ENEMY;
        enemy->size_x = 30;
        enemy->size_y = 20;
        enemy->pos_x = rand() % (SCREEN_WIDTH - 60) + 30;
        enemy->pos_y = -10;
        
        // Use random image
        int r = rand() % 5 + 1;
        drawable *sprite;
        if (r == 1) sprite = ufo1;
        if (r == 2) sprite = ufo2;
        if (r == 3) sprite = ufo3;
        if (r == 4) sprite = ufo4;
        if (r == 5) sprite = ufo5;
        component_add(enemy, component_sprite, sprite);
        
        enemy->hp = 30;
        component_add(enemy, component_hpbar, (int[]) {enemy->hp,5}) ;
        component_add(enemy, component_zigzag, &(component_zigzag_data){2, 50});
        component_add(enemy, component_move, &(component_move_data){0, 1});
        component_add(enemy, component_offscreen, &ufo_offscreen);
        component_add(enemy, component_death, &death_print);
        engine_gameobject_add(enemy);
    }
}

void powerup_spawner(){
    //Spawn enemy every 100th tick 
    if(TICK % 500 == 50){
     // Add object
        gameobject *powerup = gameobject_create();
        powerup->type = TYPE_NONE;
        powerup->size_x = 30;
        powerup->size_y = 20;
        powerup->pos_x = rand() % (SCREEN_WIDTH - 60) + 30;
        powerup->pos_y = -10;
        
        // Use random image
        drawable *sprite;
        sprite = power_sprite;

        // Add collision effect
        component_collision_data data1 = {
            .target_type = TYPE_PLAYER1,
            .self_effect = component_gameobject_remove,
            .self_param = NULL,
            .other_effect = component_damage,
            .other_param = (void*)(-3),
        };
        //Somewhat dirty fix to hit both. Should rework the component instead
        component_collision_data data2 = {
            .target_type = TYPE_PLAYER2,
            .self_effect = component_gameobject_remove,
            .self_param = NULL,
            .other_effect = component_damage,
            .other_param = (void*)(-3),
        };
        component_add(powerup, component_collision, &data1);
        component_add(powerup, component_collision, &data2);
        component_add(powerup, component_sprite, sprite);
        component_add(powerup, component_move, &(component_move_data){0, 2});
        engine_gameobject_add(powerup);
    }
}

void death_print(gameobject *object){
    printf("YO I'M a DEATH FUNCTION\n");
}

void ufo_offscreen(gameobject *object) {
    component_add(player1, component_damage, (void*)5);
    component_add(player2, component_damage, (void*)5);
    engine_gameobject_remove(object);
}

void player_death(gameobject *object) {
    screen_fill(255,0,0);
    if (object == player1) {
        screen_draw_text(20, 20, f_large, "RABBIT 1 LOSES");
    } else {
        screen_draw_text(20, 20, f_large, "RABBIT 2 LOSES");
    }
    screen_update_all();
    //ENGINE_RUNNING = 0;
    sleep(3);
}
