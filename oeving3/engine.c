#include "engine.h"
#include "graphics.h"
#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>

//How to use:
//Call setup_engine()
//Add any starting game objects you want with add_game_object
//Call run_engine()
//Any other order will segfault.



//A very simple run method without concurrency or anything fancy
void run_engine(){
    int32_t tick_nr;
    tick_nr = 1;
    engine_running = 1;
    while(engine_running){
        tick(tick_nr);
        draw();
        tick_nr++;
    }
    free_engine();
}

//Ok, this is how I think the main game logic loop should look like
void tick(int32_t tick_nr){
	int i;
    for(i = 0; i < n_game_objects; i++){
		int j;
        for(j = 0; j < game_objects[i]->n_components; j++){
            game_objects[i]->component_array[j](tick_nr, game_objects[i],j);
        }
    }
}

//And then there needs to be a drawing method
void draw(){
	int i;
    screen_fill(0,0,0);
    for(i = 0; i < draw_queue_length; i++){
        //put current gameobjects image in some screen buffer array at correct position.
		screen_draw_bmp(draw_queue[i]->pos.x, draw_queue[i]->pos.y, draw_queue[i]->image);
    }
    screen_update_all();
	draw_queue_length = 0;
    //Maybe swap directly here to the screen
}


void setup_engine(){
    draw_queue_capacity = 10; //TODO: Magic number
    draw_queue = malloc(sizeof(drawable*)*draw_queue_capacity);

    game_objects_capacity = 10; //TODO: Magic number
    game_objects = malloc(sizeof(game_object*)*game_objects_capacity);


    //set up screen
    screen_init();
}

void free_engine(){
    //TODO: Free everything!
}



int add_component(game_object* g_o, component_update component_func){
    if(g_o->n_components >= g_o->component_capacity){
        g_o->component_capacity *= 2;
        g_o-> component_array = realloc(g_o->component_array, sizeof(component_update) * g_o->component_capacity);
    }
    g_o->component_array[g_o->n_components] = component_func;
    g_o->n_components += 1;
    return g_o->n_components - 1;
}


void add_game_object(game_object* g_o){
    if(game_objects_capacity >= n_game_objects){
        game_objects_capacity *= 2;
        game_objects = realloc(game_objects, sizeof(game_object*)*game_objects_capacity);
    }
    game_objects[n_game_objects] = g_o;
    n_game_objects+=1;
}


game_object* create_game_object(){
    game_object* go;
    go = malloc(sizeof(game_object));
    go->component_capacity = 5; //TODO: Remove magic number?
    go->component_array = malloc(sizeof(component_update)*go->component_capacity);
    go->component_data = malloc(sizeof(void*)*go->component_capacity);

    return go;
}

drawable* create_drawable(bmp_image* image){
    drawable* drawing;
    drawing = malloc(sizeof(drawable));
    drawing->image = image;
    return drawing;
}

void draw_queue_append(drawable* drawing){
    if(draw_queue_length >= draw_queue_capacity ){
        draw_queue_capacity *= 2;
        draw_queue = realloc(draw_queue, sizeof(drawable*)*draw_queue_capacity);
    }
    draw_queue[draw_queue_length] = drawing;
    draw_queue_length+=1;
}
