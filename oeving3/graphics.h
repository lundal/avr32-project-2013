#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "bmp.h"
#include "fonts.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_BPP 3
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_BPP)

// Prototypes
void screen_init();
void screen_dispose();
void screen_fill(char r, char g, char b);
void screen_draw_rect(int x, int y, int width, int height, char r, char g, char b);
void screen_draw_bmp(int x, int y, bmp_image *image);
void screen_draw_text(int x, int y, FONT font, char *text, int spacing);
void screen_draw_text_background(int x, int y, FONT font, char *text, int spacing, int padding, char r, char g, char b);
void screen_update_rect(int x, int y, int width, int height);
void screen_update_all();

#endif
