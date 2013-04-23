#include "components.h"

#include "engine.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

// *****************************************************************************
// *** Example component
// *****************************************************************************
component *component_example;

// Function that is called when the component is added
void component_example_add(int component_nr, gameobject *object, void *param) {
    return;
}

// Function that is called each tick
void component_example_tick(int component_nr, gameobject *object, void *param) {
    return;
}

// Function that is called when the component is removed
void component_example_remove(int component_nr, gameobject *object, void *param) {
    return;
}



// *****************************************************************************
// *** Controllable component
// *****************************************************************************
component *component_controllable;

// Function that is called when the component is added
void component_controllable_add(int component_nr, gameobject *object, void *param) {
    return;
}

// Function that is called each tick
void component_controllable_tick(int component_nr, gameobject *object, void *param) {
    object->pos_x = TICK % SCREEN_WIDTH;
}

// Function that is called when the component is removed
void component_controllable_remove(int component_nr, gameobject *object, void *param) {
    return;
}



// *****************************************************************************
// *** Sprite component
// *****************************************************************************
component *component_sprite;

// Function that is called when the component is added
void component_sprite_add(int component_nr, gameobject *object, void *param) {
    char *image = (char*)param;
    object->components_data[component_nr] = (void*)drawable_create(bmp_load(image));
}

// Function that is called each tick
void component_sprite_tick(int component_nr, gameobject *object, void *param) {
    drawable *sprite = (drawable*)object->components_data[component_nr];
    sprite->pos_x = object->pos_x;
    sprite->pos_y = object->pos_y;
    engine_drawable_add(sprite);
}

// Function that is called when the component is removed
void component_sprite_remove(int component_nr, gameobject *object, void *param) {
    return; // TODO: Dispose sprite?
}
