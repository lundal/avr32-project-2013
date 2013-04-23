#include "engine.h"
#include "graphics.h"
#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

// Files used for io
FILE *buttons_file;
FILE *leds_file;

// Gameobjects
int gameobjects_size;
int gameobjects_capacity;
gameobject **gameobjects;

// Draw queue
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
    while (ENGINE_RUNNIG) {
        engine_tick();
        engine_draw();
        TICK++;
    }
}

// 
void engine_tick() {
    // Loop though all gameobjects
	int i;
    for (i = 0; i < gameobjects_size; i++){
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
        //put current gameobjects image in some screen buffer array at correct position.
		screen_draw_bmp(draw_queue[i]->pos.x, draw_queue[i]->pos.y, draw_queue[i]->image);
    }
    
    // Update screen
    screen_update_all();
    
    // Reset draw queue
	draw_queue_size = 0;
}

/* ************************************************************************************* */

void engine_drawing_add(drawable *drawing) {
    // Expand array if needed
    if (draw_queue_size == draw_queue_capacity) {
        draw_queue_capacity *= 2;
        draw_queue = realloc(draw_queue, sizeof(drawable*) * draw_queue_capacity);
    }
    
    // Add to queue
    draw_queue[draw_queue_size++] = drawing;
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

drawable* drawable_create(bmp_image *image) {
    drawable* drawing;
    drawing = malloc(sizeof(drawable));
    drawing->image = image;
    drawing->pos_x = 0;
    drawing->pos_y = 0;
    return drawing;
}

/* ************************************************************************************* */


int button_down(int button_nr){
    if(button_nr > 7 || button_nr < 0){
        return -1;
    }
    char buffer[1];//TODO Replace with 1 after driver has been rewritten
    fread(buffer, 1, 1, button_file);
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
    fwrite(buffer, 1, 1, led_file);
}

void led_off(int led_nr){
    if(led_nr > 7 || led_nr < 0){
        return;
    }
    char buffer[1];
    buffer[0] = 'a'+led_nr;
    fwrite(buffer, 1, 1, led_file);
}
