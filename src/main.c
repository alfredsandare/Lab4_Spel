#include "tftmd307.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <keypad.h>
#include <systick.h>
#include <gpio.h>

// In Simserver, connect 
// GPIO E(0-15)/SPI Display -> 18 LC/TFT Display

int SPACESHIP_SIZE = 40;
int SPACESHIP_XPOS = 40;
int BULLETS_SIZE = 10;


int YPOS_ENEMY_SPAWN[] = {};

int main(void) {
    // Initialize the display
    tft_init(TFT_INIT_ILI9488);
    init_gpio();
    
    int x_pos = SPACESHIP_XPOS;
    int y_pos = 180;

    // initiera tomma arrays för kulornas positioner
    int bullets_hit[2] = {0};
    int bullets_xpos[2] = {0};
    int bullets_ypos[2] = {0};

    int player_health = 5;
    int running = 1;

    // tilldela y-positioner till kulorna
    for (int i = 0; i<2; i++) {
        bullets_xpos[i] = (LCD_W - SPACESHIP_XPOS) * i/2;
        bullets_ypos[i] = y_pos;
    }

    // initiera tomma arrays för fiendernas positioner
    int enemy_xpos[3] = {0};
    int enemy_ypos[3] = {0};
    int enemy_spawn_delays[3] = {0};

    for (int i = 0; i<3; i++) {
        enemy_spawn_delays[i] = 60 * i;
        enemy_ypos[i] = 40;
        enemy_xpos[i] = 480;
    }

    while (running == 1) {
        
        // Clear the screen with black color
        // Spaceship, posistion and movement
        uint8_t key_pressed = keypad();
        switch (key_pressed) {
            case 0:
                if(y_pos > 0 +SPACESHIP_SIZE/2){
                    y_pos-=3;
                }
                break;
            case 2:
                if(y_pos < 480-SPACESHIP_SIZE/2){
                    y_pos+=3;
                }
                break;
            default:
                break;
        }
        // reset screen
        tft_lcd_rect(1, 1, LCD_W - 1, LCD_H - 1, 0x0000, 1);
        tft_lcd_rect(1,1,5,((LCD_H-1)/5)*player_health,TFT_GREEN,1);
        // Bullets, position and movement 
        for (int i = 0; i<2; i++) {
            bullets_xpos[i] += 8;
            if (bullets_xpos[i] > 480) {
                bullets_xpos[i] = SPACESHIP_XPOS+SPACESHIP_SIZE/2;
                bullets_ypos[i] = y_pos;
                bullets_hit[i] = 0;
            }
            if(bullets_hit[i]==1){
                bullets_ypos[i]=0;
            }
            if(bullets_hit[i]==0){
                tft_lcd_rect(bullets_xpos[i]-BULLETS_SIZE/2, bullets_ypos[i]-BULLETS_SIZE/2, BULLETS_SIZE, BULLETS_SIZE, TFT_WHITE, 1);
            }
        }

        for (int i = 0; i<3; i++) {
            enemy_xpos[i] -= 2;
            if (enemy_xpos[i] < 0) {
                enemy_xpos[i] = 480;
                player_health -= 1;
            }
            if (enemy_spawn_delays[i] == 0) {
                tft_lcd_rect(enemy_xpos[i], enemy_ypos[i], 20, 20, TFT_RED, 1);
            } else {
                enemy_spawn_delays[i]--;
                if (enemy_spawn_delays[i] == 0) {
                    enemy_xpos[i] = 480;
                    enemy_ypos[i] = (LCD_H/10)* (rand() % 10);
                }
            }

            for (int j=0; j<2; j++) {
                if (enemy_xpos[i] < bullets_xpos[j] && bullets_xpos[j] < enemy_xpos[i]+20 &&
                    enemy_ypos[i]-5 < bullets_ypos[j] && bullets_ypos[j] < enemy_ypos[i]+25) {
                    bullets_hit[j]=1;
                    enemy_spawn_delays[i] = (rand() % 50) + 30;
                    enemy_xpos[i] = 480;
                    
                }
            }
        }

        tft_lcd_rect(x_pos-SPACESHIP_SIZE/2, y_pos-SPACESHIP_SIZE/2, SPACESHIP_SIZE, SPACESHIP_SIZE, TFT_CYAN, 1);
        
        if(player_health == 0){
            running = 0;
        }
        // delay_ms(100000000);
    }
}