#include "tftmd307.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <keypad.h>
#include <systick.h>
#include <gpio.h>

// In Simserver, connect 
// GPIO E(0-15)/SPI Display -> 18 LC/TFT Display


int main(void) {
    // Initialize the display
    tft_init(TFT_INIT_ILI9488);
    init_gpio();
    
    int x_pos = 240;
    int y_pos = 200;

    while (1 == 1) {

        // Clear the screen with black color
        
        uint8_t key_pressed = keypad();
        switch (key_pressed) {
            case 0:
            x_pos--;
            break;
            case 2:
            x_pos++;
            break;
            default:
            break;
        }
        tft_lcd_rect(0, 0, LCD_W - 1, LCD_H - 1, 0x0000, 1);
        tft_lcd_rect(x_pos-20, y_pos-20, 40, 40, TFT_CYAN, 1);
        delay_ms(1000);
    }
}