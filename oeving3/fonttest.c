#include "bmp.h"
#include "font.h"
#include "graphics.h"

#include <stdio.h>

int main() {
    // Init
    screen_init();
    
    // Load font
    font *f1 = font_load("font_small");
    font_tint(f1, 0, 255, 0);
    font_background_set(f1, 64, 64, 64); 
    
    // Load font
    font *f2 = font_load("font_large");
    font_tint(f2, 0, 255, 255);
    font_background_set(f2, 128, 128, 128); 
    f2->spacing = 2;
    f2->padding = 2;
    
    // Try to draw text
    int i;
    for (i = 0; i < 220; i += 1) {
        /*
        screen_draw_text_background(10, i, font, "EXTERMINATE!", 1, 1, 64, 64, 64);
        screen_draw_text(10, i, font, "EXTERMINATE!", 1);
        
        screen_draw_text_background(120, i, font2, "EXTERMINATE!", 2, 2, 128, 128, 128);
        screen_draw_text(120, i, font2, "EXTERMINATE!", 2);
        */
        screen_draw_text(10, i, f1, "EXTERMINATE!");
        screen_draw_text(120, i, f2, "EXTERMINATE!");
        screen_update_rect(0, i-1, SCREEN_WIDTH, 18);
    }
    
    // Dispose
    screen_dispose();

    return 0;
}

