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
    
    // Try to draw text
    int i;
    for (i = 0; i < 230; i += 1) {
        screen_draw_text_background(0, i, font2, "EXTERMINATE!", 255, 2, 255);
        screen_draw_text(0, i, font2, "EXTERMINATE!");
        screen_update_rect(0, i, 100, 11);
        screen_update_all();
    }
    
    // Dispose
    screen_dispose();

    return 0;
}

