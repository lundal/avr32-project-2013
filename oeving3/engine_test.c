#include "engine.h"
#include "components.h"
#include "font.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    engine_init();
    
    // Init components
    components_init();
    
    // Load images
    bmp_image *img_rabby = bmp_load("rabbit.bmp");
    bmp_image *img_rabby_red = bmp_copy(img_rabby);
    bmp_tint(img_rabby_red, 255, 128, 128);
    
    // Load fonts
    font *f_small = font_load("font_small");
    font *f_large = font_load("font_large");
    
    // Create drawables
    drawable *lol = drawable_create_text(f_small, "LOL");
    drawable *rabby = drawable_create_bmp(img_rabby);
    drawable *rabby_red = drawable_create_bmp(img_rabby_red);
    
    // Add object
    gameobject *player1 = gameobject_create();
    player1->pos_y = 200;
    component_add(player1, component_player_control, (void*)0);
    component_add(player1, component_sprite, rabby);
    component_add(player1, component_shoot, rabby_red);
    component_add(player1, component_hpbar,(int[]) {30,5}) ;
    player1->hp = 20;
    engine_gameobject_add(player1);
    
    // Add object
    gameobject *enemy = gameobject_create();
    enemy->type = TYPE_ENEMY;
    enemy->pos_x = 200;
    enemy->pos_y = 20;
    enemy->size_x = 32;
    enemy->size_y = 32;
    component_add(enemy, component_sprite, rabby);
    component_add(enemy, component_hpbar, (int[]) {30,5}) ;
    enemy->hp = 20;
    engine_gameobject_add(enemy);

    gameobject *player2 = gameobject_create();
    player2->pos_y = 200;
    component_add(player2, component_player_control, (void*)1);
    component_add(player2, component_sprite, rabby);
    component_add(player2, component_shoot, rabby_red);
    component_add(player2, component_hpbar, (int[]) {30,5}) ;
    player2->hp = 20;
    engine_gameobject_add(player2);
    
    engine_run();
    
    return 0;
}
