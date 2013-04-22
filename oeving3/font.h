#ifndef FONT_HEADER
#define FONT_HEADER

#include "bmp.h"

#define FONT_NUM_CHARS 0x80

typedef struct {
    bmp_image **bitmaps;
    int spacing;
    int padding_top;
    int padding_bottom;
    int padding_left;
    int padding_right;
    char background_r;
    char background_g;
    char background_b;
} font;

// Prototypes
font *font_load(char *folder);
font *font_copy(font *f);
void font_tint(font *f, char r, char g, char b);
void font_background_set(font *f, char r, char g, char b);
void font_padding_set(font *f, int top, int bottom, int left, int right);

#endif
