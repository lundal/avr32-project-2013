//Just a rough outline of a structure that I think would be fun to implement
//Not guaranteed to compile or run
//Written late at night just to see how it could look, and to communicate my ideas
//
typedef struct{
    int32_t x;
    int32_t y;
} point;

//Typedefs a function pointer.
typedef int32_t (*componentUpdate)(int32_t tick, game_object object);

typedef struct {
    //Array of pixels or something that can be drawn;
    //Should be loaded in with a bmp parser maybe?
    int32_t** drawable;
    //Current position
    point pos;
    //hp might be relevant for some components.
    int32_t hp;
    //Array of components
    //TODO: What should the initial size be? Should we dynamically expand it?
    component_update[10] = {NULL}
    int32_t n_components; 
}game_object;


game_object** game_objects;
int32_t n_game_objects = 0;
    
//Above this line should go in the .h file. but, meh, I'll do it later

//Ok, this is how I think the main game logic loop should look like
void tick(int32_t tick_nr){
    for(int i = 0; i < n_game_objects; i++){
        for(int j = 0; j < game_objects[i]->n_components; j++){
            game_objects[i]->component_update[j](tick_nr, game_objects[i]);
        }
    }
}

//And then there needs to be a drawing method
void draw(){
    for(int i = 0; i < n_game_objects; i++){
        //put current gameobjects image in some screen buffer array at correct position.
    }

    //Maybe swap directly here to the screen
}


void setup_engine(){
    //Load up game objects and components
}


//A very simple run method without concurrency or anything fancy
void run_engine(){
    int32_t tick_nr;
    tick_nr = 1;
    while(true){
        tick(tick_nr);
        draw();
        tick_nr++;
    }
}
