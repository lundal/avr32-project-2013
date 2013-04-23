#include "graphics.h"
#include "font.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    // Read ip address
    FILE *ip_file;
    ip_file = popen("ifconfig | grep 'Bcast' | awk '{print $2}'", "r");
    char ip[100];
    fgets(ip, 100, ip_file);
    pclose(ip_file);
    
    // Init screen
    screen_init();
    
    // Load font
    font *f = font_load("font_small");
    
    // Print ip
    screen_draw_text(10,10, f, ip);
    
    // Dispose screen
    screen_dispose();
    
    return 0;
}
