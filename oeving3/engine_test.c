#include "engine.h"
#include "components.h"
#include "font.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

//defines
void death_print(gameobject *object);
void enemy_spawner();
drawable *rabby_red;
drawable *ufo1, *ufo2, *ufo3, *ufo4, *ufo5;

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
    bmp_image *img_rabby_red = bmp_copy(img_rabby);
    bmp_tint(img_rabby_red, 255, 128, 128);
    
    // Load fonts
    font *f_small = font_load("font_small");
    font *f_large = font_load("font_large");
    
    // Create drawables
    ufo1 = drawable_create_bmp(img_ufo1);
    ufo2 = drawable_create_bmp(img_ufo2);
    ufo3 = drawable_create_bmp(img_ufo3);
    ufo4 = drawable_create_bmp(img_ufo4);
    ufo5 = drawable_create_bmp(img_ufo5);
    drawable *lol = drawable_create_text(f_small, "LOL");
    drawable *rabby = drawable_create_bmp(img_rabby);
    drawable *bullet = drawable_create_rect(5, 5, 255,255,255);
    rabby_red = drawable_create_bmp(img_rabby_red);
    
    // Add object
    gameobject *player1 = gameobject_create();
    player1->pos_y = 200;
    component_add(player1, component_player_control, (void*)0);
    component_add(player1, component_sprite, rabby);
    component_add(player1, component_shoot, bullet);
    component_add(player1, component_hpbar,(int[]) {30,5}) ;
    player1->hp = 20;
    engine_gameobject_add(player1);
    
    // Add object

    gameobject *player2 = gameobject_create();
    player2->pos_y = 200;
    component_add(player2, component_player_control, (void*)1);
    component_add(player2, component_sprite, rabby);
    component_add(player2, component_shoot, bullet);
    component_add(player2, component_hpbar, (int[]) {30,5}) ;
    player2->hp = 20;
    engine_gameobject_add(player2);
    
    //Add enemy spawner
    engine_ticker_add(&enemy_spawner);

    engine_run();
    
    return 0;
}


void enemy_spawner(){
    //Spawn enemy every 100th tick 
    if(TICK % 200 == 0){
     // Add object
        gameobject *enemy = gameobject_create();
        enemy->type = TYPE_ENEMY;
        enemy->pos_x = rand() % SCREEN_WIDTH;
        enemy->pos_y = rand() % SCREEN_HEIGHT;
        enemy->size_x = 30;
        enemy->size_y = 20;
        
        // Use random image
        int r = rand() % 4 + 2;
        drawable *sprite;
        if (r == 2) sprite = ufo2;
        if (r == 3) sprite = ufo3;
        if (r == 4) sprite = ufo4;
        if (r == 5) sprite = ufo5;
        component_add(enemy, component_sprite, rabby_red);
        
        enemy->hp = 30;
        component_add(enemy, component_hpbar, (int[]) {enemy->hp,5}) ;
        component_add(enemy, component_zigzag, &(component_zigzag_data){2, 50});
        component_add(enemy, component_death, &death_print);
        engine_gameobject_add(enemy);
    }
}


void death_print(gameobject *object){
    printf("YO I'M a DEATH FUNCTION\n");
}



