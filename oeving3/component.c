#include "component.h"

#include <stdlib.h>

// Creates a new component
component *component_create(component_function add, component_function tick, component_function remove) {
    // Allocate memory
    component *c = malloc(sizeof(component));
    
    // Set functions
    c->add_function = add;
    c->tick_function = tick;
    c->remove_function = remove;
    
    return c;
}

// Frees the memory used by a component
void component_dispose(component *c) {
    return; //TODO: Implement
}

// Adds a component to a gameobject
// Return number of added component
int component_add(gameobject *object, component *c, void *param) {
    // Expand arrays if needed
    if (object->components_size == object->components_capacity) {
        object->components_capacity *= 2;
        object->components = realloc(object->components, sizeof(component_function) * object->components_capacity);
        object->components_data = realloc(object->components_data, sizeof(void*) * object->components_capacity);
    }
    
    // Get nr (and increase index)
    int nr = object->components_size++;
    
    // Insert component
    object->components[nr] = c;
    
    // Call the component's add function
    c->add_function(nr, object, param);
    
    // Return nr
    return nr;
    
    // TODO: How to prevent tick-glitching?
}

// Finds the component number of component in gameobject
// Return -1 if the component was not found
int component_find(gameobject *object, component *c) {
    // For each component in object
    int i;
    for (i = 0; i < object->components_size; i++) {
        // Get component
        component *c2 = object->components[i];
        
        // If match
        if (c == c2) {
            // Return index
            return i;
        }
    }
    
    // None was found
    return -1;
}

// Removes a component from a gameobject
// Return number of removed component
int component_remove(gameobject *object, component *c, void *param) {
    // Find number
    int num = component_find(object, c);
    
    // Check for error
    if (num < 0) {
        return num;
    }
    
    // Call the component's remove function
    c->remove_function(num, object, param);
    
    // Remove component
    object->components[num] = object->components[object->components_size - 1];
    object->components_data[num] = object->components_data[object->components_size - 1];
    object->components_size--;
    
    return num;
    
    // TODO: How to prevent tick-glitching?
}

// Creates a new gameobject
gameobject* gameobject_create() {
    // Allocate struct
    gameobject *object = malloc(sizeof(gameobject));
    
    // Set initial capacity
    object->components_size = 0;
    object->components_capacity = COMPONENT_INITIAL_SIZE;
    
    // Allocate arrays
    object->components = malloc(sizeof(component_function) * object->components_capacity);
    object->components_data = malloc(sizeof(void*) * object->components_capacity);
    
    return object;
}

// Frees the memory used by a gameobject
void gameobject_dispose(gameobject *object) {
    return; // TODO: Actually free some stuff
}
