#include "components.h"

#include "engine.h"
#include "graphics.h"
#include "component.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// *****************************************************************************
// *** Go up and dissapear
// *****************************************************************************
component *component_upup;

// Function that is called when the component is added
void component_upup_add(int component_nr, gameobject *object, void *param) {
    return;
}

// Function that is called each tick
void component_upup_tick(int component_nr, gameobject *object, void *param) {
    // Go up
    object->pos_y -= 5;
    
    // Edge check
    if (object->pos_y < 0 - object->size_y) {
        engine_gameobject_remove(object);
    }
}

// Function that is called when the component is removed
void component_upup_remove(int component_nr, gameobject *object, void *param) {
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
        gameobject *bullet = gameobject_create();
        drawable *sprite = (drawable*)object->components_data[component_nr];
        component_add(bullet, component_sprite, sprite);
        component_add(bullet, component_upup, sprite);
        component_add(bullet, component_affect_enemy, NULL);
        bullet->type = TYPE_BULLET;
        bullet->pos_x = object->pos_x;
        bullet->pos_y = object->pos_y;
        bullet->size_x = 32;
        bullet->size_y = 32;
        engine_gameobject_add(bullet);
    }
}

// Function that is called when the component is removed
void component_shoot_remove(int component_nr, gameobject *object, void *param) {
    return;
}



// *****************************************************************************
// *** Affect enemy with component
// *****************************************************************************
component *component_affect_enemy;

// Function that is called when the component is added
void component_affect_enemy_add(int component_nr, gameobject *object, void *param) {
    // Store component
    object->components_data[component_nr] = param;
    
    return;
}

// Function that is called each tick
void component_affect_enemy_tick(int component_nr, gameobject *object, void *param) {
    int i;
    for (i = 0; i < gameobjects_size; i++) {
        // Get object
        gameobject *target = gameobjects[i];
        
        // If object is an enemy
        if (target->type == TYPE_ENEMY) {
            // Check for collision
            if ( (object->pos_x < target->pos_x + target->size_x) &&
                 (object->pos_x + object->size_x > target->pos_x) &&
                 (object->pos_y < target->pos_y + target->size_y) &&
                 (object->pos_y + object->size_y > target->pos_y) ) {
                // Get stored component
                component *c = (component*)object->components_data[component_nr];
                
                // Add it to enemy
                //component_add(target, (component*)c, NULL);
                
                // Remove target
                engine_gameobject_remove(target);
                
                // Remove self?
                engine_gameobject_remove(object);
                
                break;
            }
        }
    }
}

// Function that is called when the component is removed
void component_affect_enemy_remove(int component_nr, gameobject *object, void *param) {
    return;
}



// *****************************************************************************
// *** Init function
// *****************************************************************************
void components_init() {
    component_upup = component_create(
        &component_upup_add,
        &component_upup_tick,
        &component_upup_remove
    );
    
    component_sprite = component_create(
        &component_sprite_add,
        &component_sprite_tick,
        &component_sprite_remove
    );
    
    component_player_control = component_create(
        &component_player_control_add,
        &component_player_control_tick,
        &component_player_control_remove
    );
    
    component_shoot = component_create(
        &component_shoot_add,
        &component_shoot_tick,
        &component_shoot_remove
    );

    component_affect_enemy = component_create(
        &component_affect_enemy_add,
        &component_affect_enemy_tick,
        &component_affect_enemy_remove
    );
}
