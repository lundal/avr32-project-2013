#include "engine.h"
#include "components.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    
    #define objects 100

    engine_init();
    
    // Init components
    components_init();
    
    // Load images
    drawable *rabby = drawable_create_bmp(bmp_load("rabbit.bmp"));
    
    for(int i = 0; i < objects; i++){
        // Add object
        gameobject *player1 = gameobject_create();
        player1->pos_y = rand() % SCREEN_HEIGHT;
        player1->pos_x = rand() % SCREEN_WIDTH;
        component_add(player1, component_sprite, rabby);
        component_add(player1, component_controllable, rabby);
        engine_gameobject_add(player1);
    }

    engine_run();
    
    return 0;
}
