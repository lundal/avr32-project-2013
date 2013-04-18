#include "fonts.h"

#include <stdio.c>
#include <stdlib.c>

// Create empty bitmap
bmp_image *font_empty = (bmp_image *)malloc(sizeof(bmp_image));
font_empty->width = 0;
font_empty->height = 0;
font_empty->data = NULL;




FONT font_load(char *folder) {
    // Allocate space
    FONT font = (FONT)malloc(sizeof(bmp_image*) * FONT_SET_SIZE);
    
    int i;
    for (i = 0; i < FONT_SET_SIZE) {
        // Determine file path
        char path[100];
        sprintf(path, "%s/%X.bmp", folder, i);
        
        // Load image
        bmp_image *image = bmp_load(path);
        
        // Return image or placeholder
        font[i] = (image == NULL) ? font_empty : image;
    }
    
    return font;
}
