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


int main(void) {
    // Initialize the display
    tft_init(TFT_INIT_ILI9488);
    init_gpio();
    
    int x_pos = SPACESHIP_XPOS;
    int y_pos = 180;

    // initiera tomma arrays
    int bullets_xpos[5] = {0};
    int bullets_ypos[5] = {0};

    // tilldela y-positioner till kulorna
    for (int i = 0; i<5; i++) {
        bullets_xpos[i] = (LCD_W - SPACESHIP_XPOS) * i/5;
        bullets_ypos[i] = y_pos;
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
                bullets_xpos[i] = SPACESHIP_XPOS+SPACESHIP_SIZE/2;;
                bullets_ypos[i] = y_pos;
            }
            tft_lcd_rect(bullets_xpos[i]-BULLETS_SIZE/2, bullets_ypos[i]-BULLETS_SIZE/2, BULLETS_SIZE, BULLETS_SIZE, TFT_RED, 1);
        }
        tft_lcd_rect(x_pos-SPACESHIP_SIZE/2, y_pos-SPACESHIP_SIZE/2, SPACESHIP_SIZE, SPACESHIP_SIZE, TFT_CYAN, 1);
        
        // delay_ms(100000000);
    }
}