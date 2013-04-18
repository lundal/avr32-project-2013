#include "bmp.h"
#include "graphics.h"

#include <stdio.h>

int main() {
    // Init
    screen_init();
    
    // Run
    printf("Loading...\n");
    bmp_image *bmp = bmp_load("test2.bmp");
    
    printf("BMP: %d, %d\n", bmp->width, bmp->height);
    
    printf("Displaying...\n");
    screen_draw_image(-50, -50, bmp);
    
    screen_draw_rect(150, 50, 128, 128, 255, 0, 255);
    
    screen_draw_image(150, 50, bmp);
    screen_draw_image(280, 200, bmp);
    
    screen_draw_rect(300, 0, 30, 20, 255, 255, 0);
    
    screen_update_rect(-50, -50, 500, 500);
    //screen_update_all();
    
    // Dispose
    screen_dispose();

    return 0;
}

