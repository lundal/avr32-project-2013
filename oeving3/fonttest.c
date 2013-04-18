#include "bmp.h"
#include "fonts.h"
#include "graphics.h"

#include <stdio.h>

int main() {
    // Init
    screen_init();
    
    // Load font
    FONT font = font_load("font_small");
    
    // Draw basic bmp
    bmp_image *bmp = bmp_load("test2.bmp");
    screen_draw_bmp(-50, -50, bmp);
    screen_update_all();
    
    
    // Try to draw text
    int i;
    for (i = 0; i < 240; i += 12) {
        screen_draw_text(0, i, font, "The rabbit army of pixie-land is coming!");
    }
    screen_update_all();
    
    // Dispose
    screen_dispose();

    return 0;
}

