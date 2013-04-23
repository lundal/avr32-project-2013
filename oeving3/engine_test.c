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
    gameobject *rabbit = gameobject_create();
    component_add(rabbit, component_shoot, target);
    component_add(rabbit, component_sprite, rabby);
    engine_gameobject_add(rabbit);
    
    engine_run();
    
    return 0;
}
