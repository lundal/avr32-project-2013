#include "pong.h"

#include <stdio.h>
#include "graphics.c"

FILE *buttons_file;

int player_width = 8;

int player1_size = 60;
int player1_pos = 90;
int player1_speed = 1;

int player2_size = 60;
int player2_pos = 90;
int player2_speed = 1;

int ball_x_start = 156;
int ball_y_start = 116;
int ball_x = 156;
int ball_y = 116;
int ball_dx = 1;
int ball_dy = 1;
int ball_size = 8;

int main() {
    // Init
    screen_init();
    buttons_file = fopen("/dev/buttons", "rb");
    
    // Run
    main_loop();
    
    // Dispose
    fclose(buttons_file);
    screen_dispose();
}

void main_loop() {
    while (1) {
        // Clear shapes
        screen_draw_rect(0, player2_pos, player_width, player2_size, 0,0,0);
        screen_draw_rect(SCREEN_WIDTH-player_width, player1_pos, player_width, player1_size, 0,0,0);
        screen_draw_rect(ball_x, ball_y, ball_size, ball_size, 0,0,0);
        
        handle_input();
        force_bounds();
        move_ball();
        
        // Draw shapes
        screen_draw_rect(0, player2_pos, player_width, player2_size, 255,255,255);
        screen_draw_rect(SCREEN_WIDTH-player_width, player1_pos, player_width, player1_size, 255,255,255);
        screen_draw_rect(ball_x, ball_y, ball_size, ball_size, 255,255,255);
        
        // Update screen
        screen_update_rect(0, 0, player_width, SCREEN_HEIGHT);
        screen_update_rect(SCREEN_WIDTH-player_width, 0, player_width, SCREEN_HEIGHT);
        screen_update_rect(ball_x-2, ball_y-2, ball_size+4, ball_size+4);
        
        //screen_update_all();
        //screen_update_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        
        // Wait?
        //usleep(1*1000);
    }
}

void handle_input() {
    // Read button states
    char buffer[8];
    fread(buffer, 1, 8, buttons_file);
    
    // Player 1
    if (buffer[1] == 'B') {
        player1_pos -= player1_speed;
    }
    if (buffer[2] == 'C') {
        player1_pos += player1_speed;
    }
    
    // Player 2
    if (buffer[5] == 'F') {
        player2_pos -= player2_speed;
    }
    if (buffer[6] == 'G') {
        player2_pos += player2_speed;
    }
}

void force_bounds() {
    int min, max;
    
    // Player 1
    min = 0;
    max = SCREEN_HEIGHT - player1_size;
    if (player1_pos < 0) {
        player1_pos = 0;
    }
    if (player1_pos > max) {
        player1_pos = max;
    }
    
    // Player 2
    min = 0;
    max = SCREEN_HEIGHT - player2_size;
    if (player2_pos < 0) {
        player2_pos = 0;
    }
    if (player2_pos > max) {
        player2_pos = max;
    }
}

void move_ball() {
    ball_x += ball_dx;
    ball_y += ball_dy;
    
    int min, max;
    
    // Left and right out
    min = 0;
    max = SCREEN_WIDTH - ball_size;
    if (ball_x < min) {
        ball_x = ball_x_start;
        ball_y = ball_y_start;
        ball_dx *= -1;
        ball_dy *= -1;
    }
    if (ball_x > max) {
        ball_x = ball_x_start;
        ball_y = ball_y_start;
        ball_dx *= -1;
        ball_dy *= -1;
    }
    
    // Left and right collision
    min = player_width;
    max = SCREEN_WIDTH - ball_size - player_width;
    if (ball_x == min) {
        if (ball_y + ball_size > player2_pos && ball_y < player2_pos + player2_size) {
            ball_x = min;
            ball_dx *= -1;
        }
    }
    if (ball_x == max) {
        if (ball_y + ball_size > player1_pos && ball_y < player1_pos + player1_size) {
            ball_x = max;
            ball_dx *= -1;
        }
    }
    
    // Top and bottom collision
    min = 0;
    max = SCREEN_HEIGHT - ball_size;
    if (ball_y < min) {
        ball_y = min;
        ball_dy *= -1;
    }
    if (ball_y > max) {
        ball_y = max;
        ball_dy *= -1;
    }
}
