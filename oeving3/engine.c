#include "engine.h"
#include "components.c"
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>

//Ok, this is how I think the main game logic loop should look like
void tick(int32_t tick_nr){
	int i;
    for(i = 0; i < n_game_objects; i++){
		int j;
        for(j = 0; j < game_objects[i]->n_components; j++){
            game_objects[i]->component_array[j](tick_nr, game_objects[i]);
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
    //Load up game objects and components
}


//A very simple run method without concurrency or anything fancy
void run_engine(){
    int32_t tick_nr;
    tick_nr = 1;
    while(1){
        tick(tick_nr);
        draw();
        tick_nr++;
    }
}


int main(){
	setup_engine();
	run_engine();
	return 0;
}
