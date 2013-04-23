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
    
    //printf("%s", ip);
    
    // Init screen
    screen_init();
    
    // Load font
    font *f = font_load("/root/font_large");
    
    // Print ip
    screen_draw_text(10,10, f, ip);
    
    // Update screen
    screen_update_all();
    
    // Dispose screen
    screen_dispose();
    
    return 0;
}
