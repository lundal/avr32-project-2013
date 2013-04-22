#include "engine.h"
#include "components.h"
#include "graphics.h"
#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>

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
    for(i = 0; i < draw_queue_length; i++){
        //put current gameobjects image in some screen buffer array at correct position.
		screen_draw_image(draw_queue[i]->pos.x, draw_queue[i]->pos.y, draw_queue[i]->image);
    }
	draw_queue_length = 0;
    //Maybe swap directly here to the screen
}


void setup_engine(){
    draw_queue_capacity = 10; //TODO: Magic number
    draw_queue = malloc(sizeof(drawable*)*draw_queue_capacity);

    game_objects_capacity = 10; //TODO: Magic number
    game_objects = malloc(sizeof(game_object*)*game_objects_capacity);
    //Load up game objects and components
}

void free_engine(){
    //TODO: Free everything!
}

void add_initial_game_objects(){
    int component_nr;
    game_object* rabbit = create_game_object();
    component_nr = add_component(rabbit, &sprite_component_update);
    sprite_component_init(component_nr,rabbit,create_drawable(bmp_load("rabbit.bmp")));
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
    if(draw_queue_capacity >= draw_queue_length){
        draw_queue_capacity *= 2;
        draw_queue = realloc(draw_queue, sizeof(drawable*)*draw_queue_capacity);
    }
    draw_queue[draw_queue_length] = drawing;
    draw_queue_length+=1;
}

//A very simple run method without concurrency or anything fancy
void start_engine(){
    setup_engine();
    add_initial_game_objects();
    int32_t tick_nr;
    tick_nr = 1;
    while(1){
        tick(tick_nr);
        draw();
        tick_nr++;
    }
    free_engine();
}


int main(){
	start_engine();
	return 0;
}
