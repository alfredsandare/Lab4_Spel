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

int YPOS_ENEMY_SPAWN[] = {}

int main(void) {
    // Initialize the display
    tft_init(TFT_INIT_ILI9488);
    init_gpio();
    
    int x_pos = SPACESHIP_XPOS;
    int y_pos = 180;

    // initiera tomma arrays för kulornas positioner
    int bullets_xpos[5] = {0};
    int bullets_ypos[5] = {0};
    
    // tilldela y-positioner till kulorna
    for (int i = 0; i<5; i++) {
        bullets_xpos[i] = (LCD_W - SPACESHIP_XPOS) * i/5;
        bullets_ypos[i] = y_pos;
    }

    // initiera tomma arrays för fiendernas positioner
    int enemy_xpos[5] = {0};
    int enemy_ypos[5] = {0};
    int enemy_spawn_delays[5] = {0};

    for (int i = 0; i<5; i++) {
        enemy_spawn_delays[i] = 60 * i;
        enemy_ypos[i] = 40;
    }

    while (1 == 1) {

        // Clear the screen with black color
        
        uint8_t key_pressed = keypad();
        switch (key_pressed) {
            case 0:
                if(y_pos > 0 +SPACESHIP_SIZE/2){
                    y_pos--;
                }
                break;
            case 2:
                if(y_pos < 480-SPACESHIP_SIZE/2){
                    y_pos++;
                }
                break;
            default:
                break;
        }
        tft_lcd_rect(1, 1, LCD_W - 1, LCD_H - 1, 0x0000, 1);
        for (int i = 0; i<5; i++) {
            bullets_xpos[i] += 3;
            if (bullets_xpos[i] > 480) {
                bullets_xpos[i] = SPACESHIP_XPOS+SPACESHIP_SIZE/2;
                bullets_ypos[i] = y_pos;
            }
            tft_lcd_rect(bullets_xpos[i]-BULLETS_SIZE/2, bullets_ypos[i]-BULLETS_SIZE/2, BULLETS_SIZE, BULLETS_SIZE, TFT_WHITE, 1);
        }

        for (int i = 0; i<5; i++) {
            enemy_xpos[i] -= 2;
            if (enemy_xpos[i] < 0) {
                enemy_xpos[i] = 480;

            }
            if (enemy_spawn_delays[i] == 0) {
                tft_lcd_rect(enemy_xpos[i], enemy_ypos[i], 20, 20, TFT_RED, 1);
            } else {
                enemy_spawn_delays[i]--;
                if (enemy_spawn_delays[i] == 0) {
                    enemy_xpos[i] = 480;
                }
            }

            for (int j=0; j<5; j++) {
                if (enemy_xpos[i] < bullets_xpos[j] && bullets_xpos[j] < enemy_xpos[i]+20 &&
                    enemy_ypos[i] < bullets_ypos[j] && bullets_ypos[j] < enemy_ypos[i]+20) {
                    enemy_spawn_delays[i] = 40;
                    enemy_xpos[i] = 480;
                }
            }
        }

        tft_lcd_rect(x_pos-SPACESHIP_SIZE/2, y_pos-SPACESHIP_SIZE/2, SPACESHIP_SIZE, SPACESHIP_SIZE, TFT_CYAN, 1);
        
        // delay_ms(100000000);
    }
}