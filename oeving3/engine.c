#include "engine.h"

#include "graphics.h"
#include "bmp.h"
#include "font.h"
#include "component.h"

#include <stdio.h>
#include <stdlib.h>


// Extern variables
int TICK;
int ENGINE_RUNNING;

// Files used for io
FILE *buttons_file;
FILE *leds_file;


// Tickers
ticker_function *tickers;
int tickers_capacity;
int tickers_size;

// Gameobjects
int gameobjects_size;
int gameobjects_capacity;
gameobject **gameobjects;
void engine_gameobject_ticker();

// Add queue
int add_queue_size;
int add_queue_capacity;
gameobject **add_queue;

// Remove queue
int remove_queue_size;
int remove_queue_capacity;
gameobject **remove_queue;

// Draw queue
int *draw_queue_x;
int *draw_queue_y;
int draw_queue_size;
int draw_queue_capacity;
drawable **draw_queue;

void engine_init() {
    // Init screen
    screen_init();
    
    // Open buttons
    buttons_file = fopen("/dev/buttons","rb");
    
    // Open leds
    leds_file = fopen("/dev/leds","wb");

    // Init tickers
    tickers_size = 0;
    tickers_capacity = ENGINE_INITIAL_SIZE;
    tickers = malloc(sizeof(ticker_function) * tickers_capacity);
    
    // Init gameobjects
    gameobjects_size = 0;
    gameobjects_capacity = ENGINE_INITIAL_SIZE;
    gameobjects = malloc(sizeof(gameobject*) * gameobjects_capacity);
    
    // Init add queue
    add_queue_size = 0;
    add_queue_capacity = ENGINE_INITIAL_SIZE;
    add_queue = malloc(sizeof(gameobject*) * add_queue_capacity);
    
    // Init remove queue
    remove_queue_size = 0;
    remove_queue_capacity = ENGINE_INITIAL_SIZE;
    remove_queue = malloc(sizeof(gameobject*) * remove_queue_capacity);

    // Init draw queue
    draw_queue_size = 0;
    draw_queue_capacity = ENGINE_INITIAL_SIZE;
    draw_queue = malloc(sizeof(drawable*) * draw_queue_capacity);
    draw_queue_x = malloc(sizeof(int) * draw_queue_capacity);
    draw_queue_y = malloc(sizeof(int) * draw_queue_capacity);

    //Add initial tickers
    engine_ticker_add(&engine_gameobject_ticker);
}

void engine_dispose() {
    // Close leds
    fclose(leds_file);
    
    // Close buttons
    fclose(buttons_file);
    
    // Dispose screen
    screen_dispose();
    
    // TODO: Free more stuff
}

// Run the main loop
void engine_run() {
    TICK = 0;
    ENGINE_RUNNING = 1;
    while (ENGINE_RUNNING) {
        engine_tick();
        engine_process_queues();
        engine_draw();
        TICK++;
    }
}

// 
void engine_tick() {
   int i;
   for(i = 0; i < tickers_size; i++){
        //tick every ticker
        tickers[i]();
   }
}

void engine_gameobject_ticker(){
    // Loop though all gameobjects
	int i;
    for (i = 0; i < gameobjects_size; i++) {
        // Get object
        gameobject *object = gameobjects[i];
        
        // Loop through all components
		int j;
        for (j = 0; j < object->components_size; j++) {
            // Call tick function
            object->components[j]->tick_function(j, object, NULL);
        }
    }
}

void engine_process_queues() {
    int i;
    
    // For each object in remove queue
    for (i = 0; i < remove_queue_size; i++) {
        // Get object
        gameobject *target = remove_queue[i];
        
        // For each gameobject
        int j;
        for (j = 0; j < gameobjects_size; j++) {
            // Get object
            gameobject *obj = gameobjects[j];
            
            // If match
            if (target == obj) {
                // Copy last gameobject to target's position and reduce size
                gameobjects[j] = gameobjects[gameobjects_size-- - 1];
                break;
            }
        }
    }
            
    // For each object in add queue
    for (i = 0; i < add_queue_size; i++) {
        // Get object
        gameobject *object = add_queue[i];
        
        // Expand array if needed
        if (gameobjects_size == gameobjects_capacity) {
            gameobjects_capacity *= 2;
            gameobjects = realloc(gameobjects, sizeof(gameobject*) * gameobjects_capacity);
        }
        
        // Add to gameobjects
        gameobjects[gameobjects_size++] = object;
    }
    
    // Reset queues
    add_queue_size = 0;
    remove_queue_size = 0;
}

// 
void engine_draw() {
    // Fill screen with black
    screen_fill(0,0,0);
    
    // Loop through the draw queue
	int i;
    for (i = 0; i < draw_queue_size; i++){
        drawable *d = draw_queue[i];
        int x = draw_queue_x[i];
        int y = draw_queue_y[i];
        
        // Draw BMP
        if (d->type == DRAWABLE_BMP) {
            screen_draw_bmp(x, y, (bmp_image*)d->data);
        }
        
        // Draw text
        if (d->type == DRAWABLE_TEXT) {
            screen_draw_text(x, y, (font*)d->param, (char*)d->data);
        }

        // Draw text
        if (d->type == DRAWABLE_RECT) {
            drawable_rect *rect = (drawable_rect*) d->data;
            screen_draw_rect(x, y, rect->width,rect->height, rect->r, rect->g, rect->b);
        }
    }
    
    // Update screen
    screen_update_all();
    
    // Reset draw queue
	draw_queue_size = 0;
}

/* ************************************************************************************* */

void engine_drawable_add(drawable *drawing, int x, int y) {
    // Expand array if needed
    if (draw_queue_size == draw_queue_capacity) {
        draw_queue_capacity *= 2;
        draw_queue = realloc(draw_queue, sizeof(drawable*) * draw_queue_capacity);
        draw_queue_x = realloc(draw_queue_x, sizeof(int) * draw_queue_capacity);
        draw_queue_y = realloc(draw_queue_y, sizeof(int) * draw_queue_capacity);
    }
    
    // Add to queue
    draw_queue[draw_queue_size] = drawing;
    draw_queue_x[draw_queue_size] = x;
    draw_queue_y[draw_queue_size] = y;
    
    draw_queue_size++;
}

void engine_ticker_add(ticker_function tickfunc) {
    // Expand array if needed
    if (tickers_size >= tickers_capacity) {
        tickers_capacity *= 2;
        tickers = realloc(tickers, sizeof(ticker_function) * tickers_capacity);
    }
    
    // Add
    tickers[tickers_size++] = tickfunc;
}

void engine_gameobject_add(gameobject *object) {
    // Expand array if needed
    if (add_queue_size == add_queue_capacity) {
        add_queue_capacity *= 2;
        add_queue = realloc(add_queue, sizeof(gameobject*) * add_queue_capacity);
    }
    
    // Add to queue
    add_queue[add_queue_size++] = object;
}

void engine_gameobject_remove(gameobject *object) {
    // Expand array if needed
    if (remove_queue_size == remove_queue_capacity) {
        remove_queue_capacity *= 2;
        remove_queue = realloc(add_queue, sizeof(gameobject*) * remove_queue_capacity);
    }
    
    // Add to queue
    remove_queue[remove_queue_size++] = object;
}

drawable* drawable_create_bmp(bmp_image *image) {
    drawable* drawing;
    drawing = malloc(sizeof(drawable));
    drawing->type = DRAWABLE_BMP;
    drawing->data = (void*)image;
    drawing->param = NULL;
    return drawing;
}

drawable* drawable_create_text(font *f, char *text) {
    drawable* drawing;
    drawing = malloc(sizeof(drawable));
    drawing->type = DRAWABLE_TEXT;
    drawing->data = (void*)text;
    drawing->param = (void*)f;
    return drawing;
}

drawable* drawable_create_rect(int width, int height, int r, int g, int b){
    drawable* drawing;
    drawing = malloc(sizeof(drawable));
    
    drawable_rect* rect = malloc(sizeof(drawable_rect));
    rect->width = width;
    rect->height = height;
    rect->r = r;
    rect->g = g;
    rect->b = b;
    
    drawing->data = rect;
    drawing->type = DRAWABLE_RECT;

    return drawing;
}

/* ************************************************************************************* */


int button_down(int button_nr){
    if(button_nr > 7 || button_nr < 0){
        return -1;
    }
    char buffer[1];//TODO Replace with 1 after driver has been rewritten
    fread(buffer, 1, 1, buttons_file);
    char mask = 1;
    mask = mask << button_nr;
    //Masks out the relevant bit
    return buffer[0] & mask;
}

void led_on(unsigned char led_nr){
    if(led_nr > 7){
        return;
    }
    fprintf(leds_file,"%c",'A'+led_nr);
    fflush(leds_file);
}

void led_off(unsigned char led_nr){
    if(led_nr > 7 || led_nr < 0){
        return;
    }
    fprintf(leds_file,"%c",'a'+led_nr);
    fflush(leds_file);
}
