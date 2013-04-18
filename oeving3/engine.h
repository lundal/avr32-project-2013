#ifndef ENGINE_HEADER
#define ENGINE_HEADER
#include <stdint.h>
#include "bmp.h"

typedef struct{
    int32_t x;
    int32_t y;
} point;


typedef struct{
	point pos;
	bmp_image* image;
} drawable;


//Forward declare gameobject to resolve circular dependency
typedef struct gameobject game_object;
//Typedefs a function pointer.
typedef void (*component_update)(int32_t tick_nr, game_object* go);

struct gameobject {
    //Array of pixels or something that can be drawn;
    //Should be loaded in with a bmp parser maybe?
    //Current position
    point pos;
    //hp might be relevant for some components.
    int32_t hp;
    //Array of components
    //TODO: What should the initial size be? Should we dynamically expand it?
    int32_t n_components; 
    component_update* component_array;
};


game_object** game_objects;
int32_t n_game_objects;
drawable** draw_queue;
int32_t draw_queue_length;
 
void tick(int32_t tick_nr);
void draw();
void setup_engine();
void run_engine();
int main();
#endif
