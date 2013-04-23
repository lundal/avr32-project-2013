#ifndef COMPONENTS_HEADER
#define COMPONENTS_HEADER

#include "component.h"

// Controllable component
extern component *component_upup;
void component_upup_add(int component_nr, gameobject *object, void *param);
void component_upup_tick(int component_nr, gameobject *object, void *param);
void component_upup_remove(int component_nr, gameobject *object, void *param);

// Sprite component
extern component *component_sprite;
void component_sprite_add(int component_nr, gameobject *object, void *param);
void component_sprite_tick(int component_nr, gameobject *object, void *param);
void component_sprite_remove(int component_nr, gameobject *object, void *param);

// Player control component
extern component *component_player_control;
void component_player_control_add(int component_nr, gameobject *object, void *param);
void component_player_control_tick(int component_nr, gameobject *object, void *param);
void component_player_control_remove(int component_nr, gameobject *object, void *param);

// Shooting component
extern component *component_shoot;
void component_shoot_add(int component_nr, gameobject *object, void *param);
void component_shoot_tick(int component_nr, gameobject *object, void *param);
void component_shoot_remove(int component_nr, gameobject *object, void *param);

// Init function
void components_init();

#endif
