#ifndef COMPONENTS_HEADER
#define COMPONENTS_HEADER

#include "component.h"

// General function pointer

typedef void (*gameobject_function)(gameobject *object);

// Components
extern component *component_gameobject_remove;
extern component *component_offscreen;
extern component *component_move;
extern component *component_zigzag;
extern component *component_sprite;
extern component *component_player_control;
extern component *component_shoot;
extern component *component_collision;
extern component *component_hpbar;
extern component *component_damage;
extern component *component_death;

typedef struct {
    int velocity;
    int rapidity;
} component_zigzag_data;

typedef struct {
    int speed_x;
    int speed_y;
} component_move_data;

typedef struct {
    int target_type;
    component *self_effect;
    void *self_param;
    component *other_effect;
    void *other_param;
} component_collision_data;

#define TYPE_NONE 0
#define TYPE_PLAYER 1
#define TYPE_ENEMY 2
#define TYPE_BULLET 3

// Init function
void components_init();

#endif
