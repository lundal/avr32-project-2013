#ifndef ENGINE_HEADER
#define ENGINE_HEADER
#include "bmp.h"

typedef struct{
    int x;
    int y;
} point;


typedef struct{
	point pos;
	bmp_image* image;
} drawable;


//Forward declare gameobject to resolve circular dependency
typedef struct gameobject game_object;
//Typedefs a function pointer.
typedef void (*component_update)(int tick_nr, game_object* go, int component_nr);

struct gameobject {
    //Array of pixels or something that can be drawn;
    //Should be loaded in with a bmp parser maybe?
    //Current position
    point pos;
    //hp might be relevant for some components.
    int hp;
    //Array of components
    //TODO: What should the initial size be? Should we dynamically expand it?
    int n_components; 
    int component_capacity;
    component_update* component_array;
    void **component_data;
};


game_object** game_objects;
int n_game_objects;
int game_objects_capacity;
drawable** draw_queue;
int draw_queue_length;
int draw_queue_capacity;
char engine_running;

void tick(int tick_nr);
void draw();

void setup_engine();
void free_engine();
void run_engine();

game_object* create_game_object();
drawable* create_drawable(bmp_image* image);
void add_game_object(game_object* g_o);
int add_component(game_object* g_o, component_update component_func); 

//I/O
int is_button_down(int button_nr);
void turn_led_on(int led_nr);
void turn_led_off(int led_nr);

int main();
void draw_queue_append(drawable* drawing);
#endif
