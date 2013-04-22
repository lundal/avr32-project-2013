#include "engine.h"
#include "components.h"

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
