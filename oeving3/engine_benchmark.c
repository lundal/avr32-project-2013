#include "engine.h"
#include "rabbit_components.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void spawner();
void collision_bench();
void memleak();

gameobject *player1;
gameobject *player2;
time_t time1, time2;
int number;

// Load images
drawable *rabby;
int main() {
    number = 0;
    engine_init();

    // Init components
    components_init();
    rabby = drawable_create_bmp(bmp_load("rabbit.bmp"));

    drawable *bg = drawable_create_bmp(bmp_load("images/space.bmp"));
    gameobject *background = gameobject_create();
    component_add(background, component_sprite, bg);

    engine_gameobject_add(background);

    engine_ticker_add(&collision_bench);
    time(&time1);
    engine_run();
    
    return 0;
}

void spawner(){
    #define RATE 1500
    #define OBJECTS 5
    if(TICK % RATE == 0){
        time(&time2);
        double diff = difftime(time2, time1);
        number += OBJECTS;
        printf("adding rabbits! Total: %d. Average fps last round: %f\n", number, (RATE/diff));
        int i;
        for(i = 0; i < OBJECTS; i++){
            // Add object
            gameobject *player1 = gameobject_create();
            player1->pos_y = rand() % SCREEN_HEIGHT;
            player1->pos_x = rand() % SCREEN_WIDTH;
            component_add(player1, component_sprite, rabby);
            //component_add(player1, component_zigzag,&((component_zigzag_data) {20, 10}));
            engine_gameobject_add(player1);
        }
        time(&time1);
    }
}

void collision_bench(){
    #define RATE 500
    #define OBJECTS 50
    if(TICK % RATE == 0){
        time(&time2);
        double diff = difftime(time2, time1);
        number += OBJECTS;
        printf("adding rabbits! Total: %d. Average fps last round: %f\n", number, (RATE/diff));
        int i;
        for(i = 0; i < OBJECTS; i++){
            // Add object
            gameobject *player1 = gameobject_create();
            player1->pos_y = rand() % SCREEN_HEIGHT;
            player1->pos_x = rand() % SCREEN_WIDTH;
            component_add(player1, component_sprite, rabby);
            component_add(player1, component_collision,&((component_collision_data) {NULL}));
            engine_gameobject_add(player1);
        }
        time(&time1);
    }
}

void memleak(){
    #define RATE 1
    #define OBJECTS 1
    if(TICK % RATE == 0){
        number += OBJECTS;
        int i;
        for(i = 0; i < OBJECTS; i++){
            // Add object
            gameobject *player1 = gameobject_create();
            player1->pos_y = rand() % SCREEN_HEIGHT;
            player1->type = TYPE_PLAYER1;
            player1->pos_x = rand() % SCREEN_WIDTH;
            component_add(player1, component_sprite, rabby);
            component_add(player1, component_move,&((component_move_data) {5,0}));
            component_add(player1, component_offscreen, &engine_gameobject_remove);
            engine_gameobject_add(player1);
        }
    }


    if(TICK % 500 == 0){
        time(&time2);
        double diff = difftime(time2, time1);
        printf("adding rabbits! Total: %d. Average fps last round: %f\n", number, (500/diff));
        time(&time1);
    }
}
