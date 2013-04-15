//Just a list of example components so you get the idea.

int32_t player_controlled(int32_t component_nr, int32_t tick_nr, game_object object){
    //Read button state
    //Switch 
}


int32_t poisoned(int32_t component_nr, int32_t tick_nr, game_object object){
    //Damages player every 10th tick
    if(tick_nr % 10 == 0){
        object.hp--;
    }
}

