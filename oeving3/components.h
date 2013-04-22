#ifndef COMPONENTS
#define COMPONENTS

void sprite_component_init(int component_nr, game_object *g_o, drawable *sprite);

void sprite_component_update(int tick_nr, game_object *object, int component_nr);
#endif
