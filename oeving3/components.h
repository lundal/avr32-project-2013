#ifndef COMPONENTS_HEADER
#define COMPONENTS_HEADER

#include "component.h"

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

// Shooting component
extern component *component_shoot;
void component_shoot_add(int component_nr, gameobject *object, void *param);
void component_shoot_tick(int component_nr, gameobject *object, void *param);
void component_shoot_remove(int component_nr, gameobject *object, void *param);

// Init function
void components_init();

#endif
