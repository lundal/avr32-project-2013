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
