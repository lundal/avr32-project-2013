#include "graphics.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

// Vars
int screen_file;
char *screen_map;
char *screen_buffer;

void screen_init() {
    // Memory map the screen
    screen_file = open("/dev/fb0", O_RDWR | O_CREAT | O_TRUNC);
    screen_map = mmap(NULL, SCREEN_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, screen_file, 0);
    
    // Create buffer
    screen_buffer = malloc(SCREEN_SIZE);
    screen_fill(0,0,0);
    screen_update_all();
}

void screen_dispose() {
    close(screen_file);
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
    // Initial (sub)pixel
    int xi = x * SCREEN_BPP;
    int yi = y;
    
    // End (sub)pixel
    int xe = xi + width * SCREEN_BPP;
    int ye = yi + height;
    
    // For every row
    for (y = yi; y < ye; y++) {
        // Determine start pixel
        int rowstart = y * SCREEN_WIDTH * SCREEN_BPP;
        
        // Write row
        for (x = xi; x < xe; x += 3) {
            screen_buffer[rowstart + x + 0] = b;
            screen_buffer[rowstart + x + 1] = g;
            screen_buffer[rowstart + x + 2] = r;
        }
    }
}

void screen_draw_image(int x, int y, bmp_image *image) {
    // Initial (sub)pixel
    int xi = x * SCREEN_BPP;
    int yi = y;
    
    // End (sub)pixel
    int xe = xi + image->width * SCREEN_BPP;
    int ye = yi + image->height;
    
    // For every row
    for (y = yi; y < ye; y++) {
        // BMP images are stored from bottom to top
        int y_image = ye - y - 1;

        // Determine start pixels
        int rowstart_screen = y * SCREEN_WIDTH * SCREEN_BPP;
        int rowstart_image = y_image * image->width * SCREEN_BPP;
        
        // Write row
        for (x = xi; x < xe; x++) {
            int x_image = x - xi;
            screen_buffer[rowstart_screen + x] = image->data[rowstart_image + x_image];
        }
    }
}

void screen_update_rect(int x, int y, int width, int height) {
    // Initial (sub)pixel
    int xi = x * SCREEN_BPP;
    int yi = y;
    
    // End (sub)pixel
    int xe = xi + width * SCREEN_BPP;
    int ye = yi + height;
    
    // For every row
    for (y = yi; y < ye; y++) {
        // Determine start pixel
        int rowstart = y * SCREEN_WIDTH * SCREEN_BPP;
        
        // Write row
        for (x = xi; x < xe; x++) {
            screen_map[rowstart + x] = screen_buffer[rowstart + x];
        }
    }
}

void screen_update_all() {
    memcpy(screen_map, screen_buffer, SCREEN_SIZE);
}
