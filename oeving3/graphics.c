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
    int xe_c = MIN(SCREEN_WIDTH * SCREEN_BPP, xe);
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

void screen_draw_bmp(int x, int y, bmp_image *image) {
    // Initial (sub)pixel
    int xi = x * SCREEN_BPP;
    int yi = y;
    
    // End (sub)pixel
    int xe = xi + image->width * SCREEN_BPP;
    int ye = yi + image->height;
    
    // Crop to screen
    int xi_c = MAX(0, xi);
    int yi_c = MAX(0, yi);
    int xe_c = MIN(SCREEN_WIDTH * SCREEN_BPP, xe);
    int ye_c = MIN(SCREEN_HEIGHT, ye);
    
    // Draw dimensions
    //int width = xe_c - xi_c;
    //int height = ye_c - yi_c;
    
    // Initial image (sub)pixel
    int xi_image = xi_c - xi;
    int yi_image = yi_c - yi; // Offset for image saved the correct way
    
    // End image (sub)pixel
    //int xe_image = xi_image + width;
    int ye_image = image->height - yi_image; // Where we actually start reading the BMP: Bottom minus what would normally be the top pixel
    
    // For every row
    for (y = yi_c; y < ye_c; y++) {
        // Calculate delta
        int dy = y - yi_c;
        
        // Determine start pixels
        int rowstart_screen = y * SCREEN_WIDTH * SCREEN_BPP;
        int rowstart_image = (ye_image - dy - 1) * image->width * SCREEN_BPP; // Read BMP from bottom and up
        
        // Write row
        for (x = xi_c; x < xe_c; x += 3) {
            // Calculate delta
            int dx = x - xi_c;
            
            // Read
            char b = image->data[rowstart_image + xi_image + dx + 0];
            char g = image->data[rowstart_image + xi_image + dx + 1];
            char r = image->data[rowstart_image + xi_image + dx + 2];
            
            // If transparent (fuchsia)
            if (r == 0xFF && g == 0x00 && b == 0xFF) {
                // Skip
                continue;
            }
            
            // Write
            screen_buffer[rowstart_screen + x + 0] = b;
            screen_buffer[rowstart_screen + x + 1] = g;
            screen_buffer[rowstart_screen + x + 2] = r;
        }
    }
}


void screen_draw_text(int x, int y, font *f, char *text) {
    int i;
    
    // Determine text length
    int length = strlen(text);
    
    // Vars to keep track of size
    int x_start = x;
    int y_max = 0;
    
    // Loop through characters
    for (i = 0; i < length; i++) {
        // Get character image
        bmp_image *image = f->bitmaps[(int)text[i]];
        
        // Check if null
        if (image == NULL) {
            // Skip
            continue;
        }
        
        // Update max y
        y_max = MAX(y_max, image->height);
        
        // Move right for next character
        x += image->width + f->spacing;
    }
    
    // Calculate deltas
    int dx = x - x_start - f->spacing;
    int dy = y_max;
    
    // Calculate start position and size
    int xi = x_start - f->padding_left;
    int yi = y - f->padding_top;
    int w = dx + f->padding_left + f->padding_right;
    int h = dy + f->padding_top + f->padding_bottom;
    
    // Draw background
    screen_draw_rect(xi, yi, w, h, f->background_r, f->background_g, f->background_b);
    
    // Reset x position
    x = x_start;
    
    // Loop through characters
    for (i = 0; i < length; i++) {
        // Get character image
        bmp_image *image = f->bitmaps[(int)text[i]];
        
        // Check if null
        if (image == NULL) {
            // Skip
            continue;
        }
        
        // Draw image
        screen_draw_bmp(x, y, image);
        
        // Move right for next character
        x += image->width + f->spacing;
    }
}

void screen_update_text(int x, int y, font *f, char *text) {
    int i;
    
    // Determine text length
    int length = strlen(text);
    
    // Vars to keep track of size
    int x_start = x;
    int y_max = 0;
    
    // Loop through characters
    for (i = 0; i < length; i++) {
        // Get character image
        bmp_image *image = f->bitmaps[(int)text[i]];
        
        // Check if null
        if (image == NULL) {
            // Skip
            continue;
        }
        
        // Update max y
        y_max = MAX(y_max, image->height);
        
        // Move right for next character
        x += image->width + f->spacing;
    }
    
    // Calculate deltas
    int dx = x - x_start - f->spacing;
    int dy = y_max;
    
    // Calculate start position and size
    int xi = x_start - f->padding_left;
    int yi = y - f->padding_top;
    int w = dx + f->padding_left + f->padding_right;
    int h = dy + f->padding_top + f->padding_bottom;
    
    // Update
    screen_update_rect(xi, yi, w, h);
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
    int xe_c = MIN(SCREEN_WIDTH * SCREEN_BPP, xe);
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
