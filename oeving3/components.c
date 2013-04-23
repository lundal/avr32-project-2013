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

// *****************************************************************************
// *** Player move component
// *****************************************************************************

component *component_player_control;

//data - NULL
void component_player_control_tick(int component_nr, gameobject *object, void* data){
    #define PLAYER_MOVE_RIGHT 2
    #define PLAYER_MOVE_LEFT 3
    #define PLAYER_MOVE_GAP 2
    int player_nr;
    player_nr = (int)object->components_data[component_nr];

    if(player_nr > 1 || player_nr < 0) return;
    if(button_down(PLAYER_MOVE_RIGHT+player_nr*PLAYER_MOVE_GAP)) { 
        object->pos_x = (object->pos_x + 1) % SCREEN_WIDTH;
    }
    if(button_down(PLAYER_MOVE_LEFT+player_nr*PLAYER_MOVE_GAP)) { 
        object->pos_x = (object->pos_x - 1) % SCREEN_WIDTH;
    }
}

//data - player number (0 or 1)
void component_player_control_add(int component_nr, gameobject *object, void* data){ 
    object->components_data[component_nr] = data;
}

void component_player_control_remove(int component_nr, gameobject *object, void* data){
    return;
}
