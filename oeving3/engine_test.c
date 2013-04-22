#include "engine.h"
#include "components.h"
#include "engine_test.h"


int main(){
    setup_engine();
    add_game_objects();
    run_engine();
}


void add_game_objects(){
    int component_nr;
    game_object* rabbit = create_game_object();
    component_nr = add_component(rabbit, &sprite_component);
    sprite_component_init(component_nr,rabbit,create_drawable(bmp_load("rabbit.bmp")));
    add_component(rabbit, &move_component);
    add_game_object(rabbit);
}
