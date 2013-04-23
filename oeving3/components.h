#ifndef COMPONENTS_HEADER
#define COMPONENTS_HEADER

#include "component.h"

// Initialize the component by:
// component_example = component_create(&component_example_add, &component_example_tick, component_example_remove);

// Example component
extern component *component_example;
void component_example_add(int component_nr, gameobject *object, void *param);
void component_example_tick(int component_nr, gameobject *object, void *param);
void component_example_remove(int component_nr, gameobject *object, void *param);

// Controllable component
extern component *component_controllable;
void component_controllable_add(int component_nr, gameobject *object, void *param);
void component_controllable_tick(int component_nr, gameobject *object, void *param);
void component_controllable_remove(int component_nr, gameobject *object, void *param);

// Sprite component
extern component *component_sprite;
void component_sprite_add(int component_nr, gameobject *object, void *param);
void component_sprite_tick(int component_nr, gameobject *object, void *param);
void component_sprite_remove(int component_nr, gameobject *object, void *param);

#endif
