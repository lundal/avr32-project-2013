#include "engine.h"
#include "components.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    engine_init();
    
    // Init components
    components_init();
    
    // Load images
    drawable *rabby = drawable_create_bmp(bmp_load("rabbit.bmp"));
    drawable *target = drawable_create_bmp(bmp_load("test.bmp"));
    
    // Add object
    gameobject *player1 = gameobject_create();
    player1->pos_y = 200;
    component_add(player1, component_player_control, (void*)0);
    component_add(player1, component_sprite, rabby);
    component_add(player1, component_shoot, rabby);
    engine_gameobject_add(player1);
    
    // Add object
    gameobject *player2 = gameobject_create();
    player2->pos_y = 200;
    component_add(player2, component_player_control, (void*)1);
    component_add(player2, component_sprite, rabby);
    engine_gameobject_add(player2);
    
    engine_run();
    
    return 0;
}
