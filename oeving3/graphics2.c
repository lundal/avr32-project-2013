#include "graphics2.h"

#include <stdio.h>
#include <stdlib.h>

// Vars
FILE *screen_file;
char *screen_buffer;

void screen_init() {
    // Open screen
    screen_file = fopen("/dev/fb0", "wb");
    
    // Create buffer
    screen_buffer = malloc(SCREEN_SIZE);
    screen_fill(0,0,0);
    screen_update();
}

void screen_dispose() {
    // Close screen
    fclose(screen_file);
}

void screen_fill(char r, char g, char b) {
    int i;
    for (i = 0; i < SCREEN_SIZE; i += 3) {
        screen_buffer[i+0] = b;
        screen_buffer[i+1] = g;
        screen_buffer[i+2] = r;
    }
}

void screen_draw_rect(int x, int y, int width, int height, char r, char g, char b) {
    int xi;
    int yi;
    int xe = x + width;
    int ye = y + height;
    for (yi = y; yi < ye; yi++) {
        for (xi = x; xi < xe; xi++) {
            int pixel = yi * SCREEN_WIDTH * SCREEN_BPP + xi * SCREEN_BPP;
            screen_buffer[pixel + 0] = b;
            screen_buffer[pixel + 1] = g;
            screen_buffer[pixel + 2] = r;
        }
    }
}

void screen_update() {
    rewind(screen_file);
    fwrite(screen_buffer, 1, SCREEN_SIZE, screen_file);
}
