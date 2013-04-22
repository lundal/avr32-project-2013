#include "engine.h"
#include "components.h"
#include "graphics.h"

void sprite_component_init(int component_nr, game_object *g_o, drawable* sprite){   
   g_o->component_data[component_nr] = sprite;
}

//Adds the drawable stored in the matching componentdata slot to the draw_queue
void sprite_component_update(int tick_nr, game_object *object, int component_nr){   
    drawable* sprite;
    sprite = (drawable*) object->component_data[component_nr];
    sprite->pos.x = object->pos.x;
    sprite->pos.y = object->pos.y;
    draw_queue_append(sprite);
}


void move_component(int tick_nr, game_object *object, int component_nr){   
	//object->pos.x = tick_nr % SCREEN_WIDTH;
	char buffer[8];
	fread(buffer, 1, 8, buttons_file);

	if (buffer[1] == 'A') { object->pos.x = (object->pos.x + 1) % SCREEN_WIDTH;}	
	if (buffer[3] == 'C') { object->pos.y = (object->pos.y - 1) % SCREEN_HEIGHT;}	
	if (buffer[5] == 'E') { object->pos.y = (object->pos.y + 1) % SCREEN_HEIGHT;}	
	if (buffer[7] == 'G') { object->pos.x = (object->pos.x - 1) % SCREEN_WIDTH;}	
}
