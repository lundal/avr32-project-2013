#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SCREEN_XMAX 320
#define SCREEN_YMAX 240
#define SCREEN_BPP 3
#define SCREEN_SIZE (SCREEN_XMAX * SCREEN_YMAX * SCREEN_BPP)

int screen_file;
char *screen_map;

void screen_init() {
    screen_file = open("/dev/fb0", O_RDWR | O_CREAT | O_TRUNC);
    screen_map = mmap(NULL, SCREEN_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, screen_file, 0);
}

void screen_dispose() {
    close(screen_file);
}

void draw_rect(int x, int y, int width, int height, char r, char g, char b) {
    int xi;
    int yi;
    int xe = x + width;
    int ye = y + height;
    for (yi = y; yi < ye; yi++) {
        for (xi = x; xi < xe; xi++) {
            int pixel = yi * SCREEN_XMAX * SCREEN_BPP + xi * SCREEN_BPP;
            screen_map[pixel + 0] = b;
            screen_map[pixel + 1] = g;
            screen_map[pixel + 2] = r;
            
        }
    }
}

int main() {
    /*
    int i;
    char buf[1000];
    for (i = 0; i < 500; i++) {
        buf[i] = 0x99;
    }
    FILE *sf = fopen("/dev/dsp", "wb");
    while (1) {
        fwrite(buf, 1, 1000, sf);
    }
    fclose(sf);
    */
    
    
    screen_init();
    
    int x1 = 50;
    int y1 = 100;
    
    int x2 = 150;
    int y2 = 100;
    
    FILE *bf = fopen("/dev/buttons", "rb");
    char buffer[8];
    
    draw_rect(0,0, 320,240, 0x00,0x00,0x00);
    
    while (1) {
        int x;
        int y;
        
        // Read buttons
        fread(buffer, 1, 8, bf);
        
        // Player 1
        x = x1;
        y = y1;
        if (buffer[0] == 'A') {
            x++;
        }
        if (buffer[1] == 'B') {
            y--;
        }
        if (buffer[2] == 'C') {
            y++;
        }
        if (buffer[3] == 'D') {
            x--;
        }
        if (x != x1 || y != y1) {
            draw_rect(x1,y1, 50,50, 0x00,0x00,0x00);
            draw_rect(x,y, 50,50, 0xFF,0x99,0xFF);
            x1 = x;
            y1 = y;
        }
        
        // Player 2
        x = x2;
        y = y2;
        if (buffer[4] == 'E') {
            x++;
        }
        if (buffer[5] == 'F') {
            y--;
        }
        if (buffer[6] == 'G') {
            y++;
        }
        if (buffer[7] == 'H') {
            x--;
        }
        if (x != x2 || y != y2) {
            draw_rect(x2,y2, 50,50, 0x00,0x00,0x00);
            draw_rect(x,y, 50,50, 0x99,0xFF,0xFF);
            x2 = x;
            y2 = y;
        }
        
        // Wait
        int i = 50000;
        while (i--) {
        }
    }
    
    fclose(bf);
    
    screen_dispose();
}
