#ifndef ENGINE_HEADER
#define ENGINE_HEADER

#include "bmp.h"
#include "font.h"
#include "component.h"

#define ENGINE_INITIAL_SIZE 8

#define DRAWABLE_BMP 0
#define DRAWABLE_TEXT 1
#define DRAWABLE_RECT 2

// External variables
extern int TICK;
extern int ENGINE_RUNNING;

// Tickers
typedef void (*ticker_function)();
extern ticker_function* tickers;
extern int tickers_size;

// Gameobjects
extern int gameobjects_size;
extern gameobject **gameobjects;

// Drawable struct
typedef struct {
	int type;
	void* data;
    void* param;
} drawable;

typedef struct {
    int width;
    int height;
    int r;
    int g;
    int b;
} drawable_rect;


// Engine prototypes
void engine_init();
void engine_dispose();
void engine_run();
void engine_tick();
void engine_process_queues();
void engine_draw();
void engine_ticker_add(ticker_function tickfunc);
void engine_gameobject_add(gameobject *object);
void engine_gameobject_remove(gameobject *object);
void engine_drawable_add(drawable *drawing, int x, int y);
drawable* drawable_create_bmp(bmp_image *image);
drawable* drawable_create_text(font *f, char *text);
drawable* drawable_create_rect(int width, int height, int r, int g, int b);


// IO
int button_down(int button_nr);
void led_on(unsigned char led_nr);
void led_off(unsigned char led_nr);

int main();

#endif
