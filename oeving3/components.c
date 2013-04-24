#include "components.h"

#include "engine.h"
#include "graphics.h"
#include "component.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// *****************************************************************************
// *** Remove gameobject
// *****************************************************************************
component *component_gameobject_remove;

// Function that is called when the component is added
void component_gameobject_remove_add(int component_nr, gameobject *object, void *param) {
    // Remove this component
    component_remove_by_nr(object, component_nr, NULL);
    
    // Remove gameobject
    engine_gameobject_remove(object);
}

// Function that is called each tick
void component_gameobject_remove_tick(int component_nr, gameobject *object, void *param) {
    return;
}

// Function that is called when the component is removed
void component_gameobject_remove_remove(int component_nr, gameobject *object, void *param) {
    return;
}


// *****************************************************************************
// *** Move in a direction and remove when offscreen
// *****************************************************************************
component *component_move_disappear;

// Function that is called when the component is added
// param: component_move_data *speed
void component_move_disappear_add(int component_nr, gameobject *object, void *param) {
    // Get data
    component_move_data data = *(component_move_data*)param;
    
    // Store data
    object->components_data[component_nr] = (void*)malloc(sizeof(component_move_data));
    *((component_move_data*)object->components_data[component_nr]) = data;
}

// Function that is called each tick
void component_move_disappear_tick(int component_nr, gameobject *object, void *param) {
    // Get data
    component_move_data *data = (component_move_data*)object->components_data[component_nr];
    
    // Move
    object->pos_y += data->speed_x;
    object->pos_y += data->speed_y;
    
    // Edge check
    if ( (object->pos_x + object->size_x < 0) || (object->pos_x >= SCREEN_WIDTH) ||
         (object->pos_y + object->size_y < 0) || (object->pos_y >= SCREEN_HEIGHT) ) {
        // Remove gameobject
        engine_gameobject_remove(object);
    }
}

// Function that is called when the component is removed
void component_move_disappear_remove(int component_nr, gameobject *object, void *param) {
    free(object->components_data[component_nr]);
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
    return;
}

// *****************************************************************************
// *** Player move component
// *****************************************************************************
component *component_player_control;

#define PLAYER_MOVE_RIGHT 2
#define PLAYER_MOVE_LEFT 3
#define PLAYER_MOVE_GAP 2

//data - player number (0 or 1)
void component_player_control_add(int component_nr, gameobject *object, void* data){ 
    object->components_data[component_nr] = data;
}

//data - NULL
void component_player_control_tick(int component_nr, gameobject *object, void* data){
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
        // Create object
        gameobject *bullet = gameobject_create();
        
        // Add sprite
        drawable *sprite = (drawable*)object->components_data[component_nr];
        component_add(bullet, component_sprite, sprite);
        
        // Make it go up
        component_move_data data1 = {
            .speed_x = 0,
            .speed_y = -5
        };
        component_add(bullet, component_move_disappear, &data1);
        
        // Add collision effect
        component_collision_data data2 = {
            .target_type = TYPE_ENEMY,
            .self_effect = component_gameobject_remove,
            .self_param = NULL,
            .other_effect = component_damage,
            .other_param = (void*)1,
        };
        component_add(bullet, component_collision, &data2);
        
        // Set basic properties
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
// *** Damage component
// *** Causes set amount of damage and removes itself
// *****************************************************************************
component *component_damage;

// Function that is called when the component is added
// param = (int) damage
void component_damage_add(int component_nr, gameobject *object, void *param) {
    object->components_data[component_nr] = param;
}

// Function that is called each tick
void component_damage_tick(int component_nr, gameobject *object, void *param) {
    int damage = (int) object->components_data[component_nr];
    object->hp -= damage;
    component_remove_by_nr(object, component_nr, NULL);
}

// Function that is called when the component is removed
void component_damage_remove(int component_nr, gameobject *object, void *param) {
    return;
}



// *****************************************************************************
// *** Affect objects on collision
// *****************************************************************************
component *component_collision;

// Function that is called when the component is added
// param: component_collision_data*
void component_collision_add(int component_nr, gameobject *object, void *param) {
    // Get data
    component_collision_data data = *(component_collision_data*)param;
    
    // Store data
    object->components_data[component_nr] = (void*)malloc(sizeof(component_collision_data));
    *((component_collision_data*)object->components_data[component_nr]) = data;
}

// Function that is called each tick
void component_collision_tick(int component_nr, gameobject *object, void *param) {
    // Retrieve data
    component_collision_data *data = object->components_data[component_nr];
    
    // For all gameobjects
    int i;
    for (i = 0; i < gameobjects_size; i++) {
        // Get object
        gameobject *target = gameobjects[i];
        
        // If object is an specified type
        if (target->type == data->target_type) {
            // Check for collision
            if ( (object->pos_x < target->pos_x + target->size_x) &&
                 (object->pos_x + object->size_x > target->pos_x) &&
                 (object->pos_y < target->pos_y + target->size_y) &&
                 (object->pos_y + object->size_y > target->pos_y) ) {
                // Affect self
                component_add(object, data->self_effect, data->self_param);
                
                // Affect other
                component_add(target, data->other_effect, data->other_param);
                
                // Remove this component
                component_remove_by_nr(object, component_nr, NULL);
                
                break;
            }
        }
    }
}

// Function that is called when the component is removed
void component_collision_remove(int component_nr, gameobject *object, void *param) {
    // Clear param
    free(object->components_data[component_nr]);
}


// *****************************************************************************
// *** HP bar component 
// *****************************************************************************
component *component_hpbar;

typedef struct {
    drawable *empty_bar;
    drawable *full_bar;
}hpbar_data;

// param - {width, height} 
// Function that is called when the component is added
void component_hpbar_add(int component_nr, gameobject *object, void *param) {
    int *input = (int*)param;
    drawable *empty_bar = drawable_create_rect(input[0],input[1],255,0,0);
    drawable *full_bar = drawable_create_rect(input[0],input[1],0,255,0);
    
    //store in struct
    object->components_data[component_nr] = malloc(sizeof(hpbar_data));
    *((hpbar_data*) object->components_data[component_nr]) = (hpbar_data){
            .empty_bar = empty_bar,
            .full_bar = full_bar
    };

}

// Function that is called each tick
void component_hpbar_tick(int component_nr, gameobject *object, void *param) {
    hpbar_data *bardata = (hpbar_data*)object->components_data[component_nr];

    //update size of full bar
    drawable_rect *rect =(drawable_rect*) bardata->full_bar->data;
    rect->width = object->hp;

    engine_drawable_add(bardata->empty_bar, object->pos_x, object->pos_y);
    engine_drawable_add(bardata->full_bar, object->pos_x, object->pos_y);
}

// Function that is called when the component is removed
void component_hpbar_remove(int component_nr, gameobject *object, void *param) {
    free(object->components_data[component_nr]);
    return; // TODO: Dispose sprite?
}




// *****************************************************************************
// *** Init function
// *****************************************************************************
void components_init() {
    component_gameobject_remove = component_create(
        &component_gameobject_remove_add,
        &component_gameobject_remove_tick,
        &component_gameobject_remove_remove
    );
    component_move_disappear = component_create(
        &component_move_disappear_add,
        &component_move_disappear_tick,
        &component_move_disappear_remove
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
    component_collision = component_create(
        &component_collision_add,
        &component_collision_tick,
        &component_collision_remove
    );
    component_hpbar = component_create(
        &component_hpbar_add,
        &component_hpbar_tick,
        &component_hpbar_remove
        );  
    component_damage = component_create(
        &component_damage_add,
        &component_damage_tick,
        &component_damage_remove
        );  
}

