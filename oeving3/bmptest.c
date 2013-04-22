#include "bmp.h"
#include "graphics.h"

#include <stdio.h>

int main() {
    // Init
    printf("Init screen...\n");
    screen_init();
    
    // Run
    printf("Loading...\n");
    bmp_image *bmp = bmp_load("test2.bmp");
    
    screen_draw_bmp(-50, -50, bmp);
    
    screen_draw_rect(150, 50, 128, 128, 255, 0, 255);
    
    bmp_image *bmp2 = bmp_copy(bmp);
    bmp_tint(bmp2, 0, 255, 0);
    screen_draw_bmp(50, 50, bmp2);
    
    bmp_image *bmp3 = bmp_copy(bmp);
    bmp_tint(bmp3, 0, 0, 255);
    screen_draw_bmp(150, 50, bmp3);
    
    screen_draw_bmp(280, 200, bmp);
    
    screen_draw_rect(300, 0, 30, 20, 255, 255, 0);
    
    screen_update_all();
    
    // Dispose
    screen_dispose();

    return 0;
}

