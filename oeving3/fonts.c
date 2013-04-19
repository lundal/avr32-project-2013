#include "fonts.h"

#include <stdio.h>
#include <stdlib.h>

FONT font_load(char *folder) {
    // Allocate space
    FONT font = (FONT)malloc(sizeof(bmp_image*) * FONT_SET_SIZE);
    
    int i;
    for (i = 0; i < FONT_SET_SIZE; i++) {
        // Determine file path
        char path[100];
        sprintf(path, "%s/%X.bmp", folder, i);
        
        // Load image
        font[i] = bmp_load(path);
    }
    
    return font;
}

FONT font_copy(FONT font) {
    // Verify font
    if (font == NULL) {
        return NULL;
    }
    
    // Allocate space
    FONT font_new = (FONT)malloc(sizeof(bmp_image*) * FONT_SET_SIZE);
    
    // For all character bitmaps
    int i;
    for (i = 0; i < FONT_SET_SIZE; i++) {
        // Add a copy
        font_new[i] = bmp_copy(font[i]);
    }
    
    return font_new;
}

void font_tint(FONT font, char r, char g, char b) {
    // Verify font
    if (font == NULL) {
        return;
    }
    
    // For all character bitmaps
    int i;
    for (i = 0; i < FONT_SET_SIZE; i++) {
        // Tint image
        bmp_tint(font[i], r, g, b);
    }
}
