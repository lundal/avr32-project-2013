#include "engine.h"
#include "components.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    engine_init();
    
    // Init components
    component_example = component_create(&component_example_add, &component_example_tick, &component_example_remove);
    component_player_control = component_create(&component_player_control_add, &component_player_control_tick, &component_player_control_remove);
    component_sprite = component_create(&component_sprite_add, &component_sprite_tick, &component_sprite_remove);
    
    // Add object
    gameobject *player1 = gameobject_create();
    player1->pos_y = 150;
    component_add(player1, component_player_control,(void*) 0);
    component_add(player1, component_sprite, "rabbit.bmp");
    engine_gameobject_add(player1);

    gameobject *player2 = gameobject_create();
    player2->pos_y = 150;
    component_add(player2, component_player_control,(void*) 1);
    component_add(player2, component_sprite, "rabbit.bmp");
    engine_gameobject_add(player2);

    engine_run();
    
    return 0;
}
