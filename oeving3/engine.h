#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include "bmp.h"
#include "font.h"
#include "component.h"

#define ENGINE_INITIAL_SIZE 8

#define DRAWABLE_BMP 0
#define DRAWABLE_TEXT 1

// External variables
extern int TICK;
extern int ENGINE_RUNNING;

typedef struct {
	int type;
	void* data;
    void* param;
} drawable;


// Engine prototypes
void engine_init();
void engine_dispose();
void engine_run();
void engine_tick();
void engine_process_queues();
void engine_draw();
void engine_gameobject_add(gameobject *object);
void engine_gameobject_remove(gameobject *object);
void engine_drawable_add(drawable *drawing, int x, int y);
drawable* drawable_create_bmp(bmp_image *image);
drawable* drawable_create_text(font *f, char *text);


// IO
int button_down(int button_nr);
void led_on(int led_nr);
void led_off(int led_nr);

int main();

#endif
