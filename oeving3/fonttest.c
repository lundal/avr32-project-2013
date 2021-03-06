#include "bmp.h"
#include "font.h"
#include "graphics.h"

#include <stdio.h>

int main() {
    int i, j;
    
    // Init
    screen_init();
    
    // Load font
    font *f1 = font_load("font_small");
    font_tint(f1, 0, 255, 0);
    font_background_set(f1, 64, 64, 64); 
    font_padding_set(f1, 2, 0, 2, 2);
    f1->spacing = 1;
    
    // Load font
    font *f2 = font_load("font_large");
    font_tint(f2, 0, 255, 255);
    font_background_set(f2, 128, 128, 128); 
    font_padding_set(f2, 4, 0, 4, 4);
    f2->spacing = 2;
    
    int h = 22;
    
    for (j = 0; j < 10; j++) {
        for (i = 0; i < 220 - j*h; i += 1) {
            screen_draw_rect(0, i-6, SCREEN_WIDTH, 2, 0, 0, 0);
            
            screen_draw_text(80, i, f2, "EXTERMINATE!");
            screen_update_text(80, i, f2, "EXTERMINATE!");
            
            screen_update_rect(0, i-6, SCREEN_WIDTH, 2);
        }
    }
    
    // Dispose
    screen_dispose();

    return 0;
}

