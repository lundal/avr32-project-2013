#include "engine.h"
#include "components.h"
#include "graphics.h"

void sprite_component_init(int component_nr, game_object *g_o, drawable* sprite){   
   g_o->component_data[component_nr] = sprite;
}

//Adds the drawable stored in the matching componentdata slot to the draw_queue
void sprite_component(int tick_nr, game_object *object, int component_nr){   
    drawable* sprite;
    sprite = (drawable*) object->component_data[component_nr];
    sprite->pos.x = object->pos.x;
    sprite->pos.y = object->pos.y;
    draw_queue_append(sprite);
}


void move_component(int tick_nr, game_object *object, int component_nr){   
	//object->pos.x = tick_nr % SCREEN_WIDTH;
	if (is_button_down(0)) { object->pos.x = (object->pos.x + 1) % SCREEN_WIDTH;}	
	if (is_button_down(2)) { object->pos.y = (object->pos.y - 1) % SCREEN_HEIGHT;}	
	if (is_button_down(4)) { object->pos.y = (object->pos.y + 1) % SCREEN_HEIGHT;}	
	if (is_button_down(6)) { object->pos.x = (object->pos.x - 1) % SCREEN_WIDTH;}	
}


//data - NULL
void player_move_component_tick(int component_nr, game_object *object, void* data){
    #define PLAYER_MOVE_RIGHT = 2
    #define PLAYER_MOVE_LEFT = 3
    #define PLAYER_MOVE_GAP = 2
    int player_nr;
    player_nr = (int)object->component_data[component_nr];

    if(player_nr > 1 || player_nr < 0) return;
    if(is_button_down(PLAYER_MOVE_RIGHT+player_nr*PLAYER_MOVE_GAP)) { 
        object->pos.x = (object->pos.x + 1) % SCREEN_WIDTH;
    }
    if(is_button_down(PLAYER_MOVE_LEFT+player_nr*PLAYER_MOVE_GAP)) { 
        object->pos.x = (object->pos.x - 1) % SCREEN_WIDTH;
    }
}

//data - player number (0 or 1)
void player_move_component_add(int component_nr, game_object *object, void* data){ 
    object->component_data[component_nr] = data;
}

void player_move_component_remove(int component_nr, game_object *object, void* data){
    return;
}
