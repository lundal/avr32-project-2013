#include "bmp.h"
#include "graphics.h"

#include <stdio.h>

int main() {
    // Init
    screen_init();
    
    // Run
    printf("Loading...\n");
    bmp_image *bmp = bmp_load("test.bmp");
    
    printf("BMP: %d, %d\n", bmp->width, bmp->height);

    printf("Displaying...\n");
    screen_draw_image(20, 20, bmp);
    screen_update_all();
    
    // Dispose
    screen_dispose();

    return 0;
}

