#include "graphics.h"

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

void screen_update_rect(int x, int y, int width, int height) {
    int xi;
    int yi;
    int xe = x + width;
    int ye = y + height;
    for (yi = y; yi < ye; yi++) {
        for (xi = x; xi < xe; xi++) {
            int pixel = yi * SCREEN_WIDTH * SCREEN_BPP + xi * SCREEN_BPP;
            screen_map[pixel + 0] = screen_buffer[pixel + 0];
            screen_map[pixel + 1] = screen_buffer[pixel + 1];
            screen_map[pixel + 2] = screen_buffer[pixel + 2];
            
        }
    }
}

void screen_update_all() {
    int i;
    for (i = 0; i < SCREEN_SIZE; i++) {
        screen_map[i] = screen_buffer[i];
    }
}
