#include "engine.h"
#include "components.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    engine_init();
    
    // Init components
    component_example = component_create(&component_example_add, &component_example_tick, component_example_remove);
    component_controllable = component_create(&component_controllable_add, &component_controllable_tick, component_controllable_remove);
    component_sprite = component_create(&component_sprite_add, &component_sprite_tick, component_sprite_remove);
    
    // Add object
    gameobject *rabbit = gameobject_create();
    component_add(rabbit, component_controllable, NULL);
    component_add(rabbit, component_sprite, "rabbit.bmp");
    engine_gameobject_add(rabbit);
    
    engine_run();
    
    return 0;
}
