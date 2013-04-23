#include "components.h"

#include "engine.h"
#include "graphics.h"
#include "component.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    object->components_data[component_nr] = (void*)param;
}

// Function that is called each tick
void component_sprite_tick(int component_nr, gameobject *object, void *param) {
    drawable *sprite = (drawable*)object->components_data[component_nr];
    engine_drawable_add(sprite, object->pos_x, object->pos_y);
}

// Function that is called when the component is removed
void component_sprite_remove(int component_nr, gameobject *object, void *param) {
    return; // TODO: Dispose sprite?
}


/*
// *****************************************************************************
// *** Shooting component
// *****************************************************************************
component *component_shoot;

// Function that is called when the component is added
void component_shoot_add(int component_nr, gameobject *object, void *param) {
    // Store bullet drawable
    object->components_data[component_nr] = (void*)param;
}

// Function that is called each tick
void component_shoot_tick(int component_nr, gameobject *object, void *param) {
    if (TICK % 20 == 0) {
        gameobject *bullet = engine_gameobject_create();
        drawable *
    }
    drawable *sprite = (drawable*)object->components_data[component_nr];
    engine_drawable_add(sprite, object->pos_x, object);
}

// Function that is called when the component is removed
void component_shoot_remove(int component_nr, gameobject *object, void *param) {
    return;
}
*/
