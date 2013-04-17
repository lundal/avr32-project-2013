#include "graphics.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

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
    
    // Crop to screen
    int xi_c = MAX(0, xi);
    int yi_c = MAX(0, yi);
    int xe_c = MIN(SCREEN_WIDTH, xe);
    int ye_c = MIN(SCREEN_HEIGHT, ye);
    
    // For every row
    for (y = yi_c; y < ye_c; y++) {
        // Determine start pixel
        int rowstart = y * SCREEN_WIDTH * SCREEN_BPP;
        
        // Write row
        for (x = xi_c; x < xe_c; x += 3) {
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
    
    // Crop to screen
    int xi_c = MAX(0, xi);
    int yi_c = MAX(0, yi);
    int xe_c = MIN(SCREEN_WIDTH, xe);
    int ye_c = MIN(SCREEN_HEIGHT, ye);
    
    // Draw dimensions
    int width = xe_c - xi_c;
    int height = ye_c - yi_c;
    
    // Initial image (sub)pixel
    int xi_image = xi_c - xi;
    int yi_image = yi_c - yi;
    
    // End image (sub)pixel
    int xe_image = xi_image + width;
    int ye_image = yi_image + height;
    
    // For every row
    for (y = yi_c; y < ye_c; y++) {
        // Calculate delta
        int dy = y - yi_c;
        
        // BMP images are stored from bottom to top
        //int y_image = ye - y - 1;

        // Determine start pixels
        int rowstart_screen = y * SCREEN_WIDTH * SCREEN_BPP;
        int rowstart_image = (ye_image - dy - 1) * image->width * SCREEN_BPP;
        
        // Write row
        for (x = xi_c; x < xe_c; x++) {
            // Calculate delta
            int dx = x - xi_c;
            
            // Write
            screen_buffer[rowstart_screen + x] = image->data[rowstart_image + xi_image + dx];
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
    
    // Crop to screen
    int xi_c = MAX(0, xi);
    int yi_c = MAX(0, yi);
    int xe_c = MIN(SCREEN_WIDTH, xe);
    int ye_c = MIN(SCREEN_HEIGHT, ye);
    
    // For every row
    for (y = yi_c; y < ye_c; y++) {
        // Determine start pixel
        int rowstart = y * SCREEN_WIDTH * SCREEN_BPP;
        
        // Write row
        for (x = xi_c; x < xe_c; x++) {
            screen_map[rowstart + x] = screen_buffer[rowstart + x];
        }
    }
}

void screen_update_all() {
    memcpy(screen_map, screen_buffer, SCREEN_SIZE);
}
