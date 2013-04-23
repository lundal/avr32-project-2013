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

// Gameobjects
int gameobjects_size;
int gameobjects_capacity;
gameobject **gameobjects;

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
    
    // Init gameobjects
    gameobjects_size = 0;
    gameobjects_capacity = 8; //TODO: Magic number
    gameobjects = malloc(sizeof(gameobject*) * gameobjects_capacity);

    // Init draw queue
    draw_queue_size = 0;
    draw_queue_capacity = 8; //TODO: Magic number
    draw_queue = malloc(sizeof(drawable*) * draw_queue_capacity);
    draw_queue_x = malloc(sizeof(int) * draw_queue_capacity);
    draw_queue_y = malloc(sizeof(int) * draw_queue_capacity);
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
        engine_draw();
        TICK++;
    }
}

// 
void engine_tick() {
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

void engine_gameobject_add(gameobject *object) {
    // Expand array if needed
    if (gameobjects_size == gameobjects_capacity) {
        gameobjects_capacity *= 2;
        gameobjects = realloc(gameobjects, sizeof(drawable*) * gameobjects_capacity);
    }
    
    // Add to queue
    gameobjects[gameobjects_size++] = object;
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

void led_on(int led_nr){
    if(led_nr > 7 || led_nr < 0){
        return;
    }
    char buffer[1];
    buffer[0] = 'A'+led_nr;
    fwrite(buffer, 1, 1, leds_file);
}

void led_off(int led_nr){
    if(led_nr > 7 || led_nr < 0){
        return;
    }
    char buffer[1];
    buffer[0] = 'a'+led_nr;
    fwrite(buffer, 1, 1, leds_file);
}
