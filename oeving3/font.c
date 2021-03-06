#include "font.h"

#include <stdio.h>
#include <stdlib.h>


font *font_load(char *folder) {
    // Allocate space for font
    font *f = (font *)malloc(sizeof(font));
    
    // Allocate space for bitmap array
    f->bitmaps = (bmp_image **)malloc(sizeof(bmp_image *) * FONT_NUM_CHARS);
    
    // For all characters
    int i;
    for (i = 0; i < FONT_NUM_CHARS; i++) {
        // Determine file path
        char path[100];
        sprintf(path, "%s/%X.bmp", folder, i);
        
        // Load bitmap
        f->bitmaps[i] = bmp_load(path);
    }
    
    // Set default spacing
    f->spacing = 1;
    
    // Set default padding
    font_padding_set(f, 2, 0, 2, 2);
    
    // Set transparent background
    font_background_set(f, 0xFF, 0x00, 0xFF);
    
    return f;
}


font *font_copy(font *f) {
    // Verify font
    if (f == NULL) {
        return NULL;
    }
    
    // Verify bitmaps
    if (f->bitmaps == NULL) {
        return NULL;
    }
    
    // Allocate space for font
    font *f_n = (font *)malloc(sizeof(font));
    
    // Allocate space for bitmap array
    f_n->bitmaps = (bmp_image **)malloc(sizeof(bmp_image *) * FONT_NUM_CHARS);
    
    // For all character bitmaps
    int i;
    for (i = 0; i < FONT_NUM_CHARS; i++) {
        // Add a copy
        f_n->bitmaps[i] = bmp_copy(f->bitmaps[i]);
    }
    
    // Copy spacing
    f_n->spacing = f->spacing;
    
    // Copy background
    f_n->background_r = f_n->background_r;
    f_n->background_g = f_n->background_g;
    f_n->background_b = f_n->background_b;
    
    // Copy padding
    f_n->padding_top = f->padding_top;
    f_n->padding_bottom = f->padding_bottom;
    f_n->padding_left = f->padding_left;
    f_n->padding_right = f->padding_right;
    
    return f_n;
}


void font_tint(font *f, char r, char g, char b) {
    // Verify font
    if (f == NULL) {
        return;
    }
    
    // Verify bitmaps
    if (f->bitmaps == NULL) {
        return;
    }
    
    // For all character
    int i;
    for (i = 0; i < FONT_NUM_CHARS; i++) {
        // Tint bitmap
        bmp_tint(f->bitmaps[i], r, g, b);
    }
}

void font_background_set(font *f, char r, char g, char b) {
    f->background_r = r;
    f->background_g = g;
    f->background_b = b;
}

void font_padding_set(font *f, int top, int bottom, int left, int right) {
    f->padding_top = top;
    f->padding_bottom = bottom;
    f->padding_left = left;
    f->padding_right = right;
}
