#ifndef COMPONENTS_HEADER
#define COMPONENTS_HEADER

#include "component.h"

// Components
extern component *component_upup;
extern component *component_sprite;
extern component *component_player_control;
extern component *component_shoot;
extern component *component_affect_enemy;

#define TYPE_NONE 0
#define TYPE_PLAYER 1
#define TYPE_ENEMY 2
#define TYPE_BULLET 3

// Init function
void components_init();

#endif
