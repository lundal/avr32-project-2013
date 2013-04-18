#include "bmp.h"
#include "fonts.h"
#include "graphics.h"

#include <stdio.h>

int main() {
    // Init
    screen_init();
    
    // Load font
    FONT font = font_load("font_small");
    FONT font2 = font_copy(font);
    font_tint(font2, 0, 255, 0);
    
    // Draw basic bmp
    bmp_image *bmp = bmp_load("test2.bmp");
    screen_draw_bmp(-50, -50, bmp);
    screen_update_all();
    
    
    // Try to draw text
    int i;
    for (i = 0; i < 240; i += 12) {
        screen_draw_text(0, i, font2, "EXTERMINATE!");
        screen_draw_text(100, i, font, "The rabbit army of pixie-land!");
    }
    screen_update_all();
    
    // Dispose
    screen_dispose();

    return 0;
}

