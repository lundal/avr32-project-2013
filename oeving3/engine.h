#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include "bmp.h"
#include "component.h"

// External variables
extern int TICK;
extern int ENGINE_RUNNING;


typedef struct{
	int pos_x;
	int pos_y;
	bmp_image* image;
} drawable;


// Engine prototypes
void engine_init();
void engine_dispose();
void engine_run();
void engine_tick();
void engine_draw();
void engine_gameobject_add(gameobject *object);
void engine_drawable_add(drawable *drawing);
drawable* drawable_create(bmp_image *image);


// IO
int button_down(int button_nr);
void led_on(int led_nr);
void led_off(int led_nr);

int main();

#endif
