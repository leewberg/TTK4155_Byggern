#pragma once
#include "oled.h"
#include "SPI.h"
#include <util/delay.h>

void oled_init(){
    SPI_select_slave(OLED_CMD);
    SPI_M_write(OLED_OFF); //OFF
    SPI_M_write(0xA1);
    SPI_M_write(0xc8);
    SPI_M_write(0x20);
    SPI_M_write(0x02);
    SPI_M_write(0xA6);
    SPI_M_write(0xA4);
    SPI_M_write(0xAf);
    SPI_M_write(0xb0);
/*    SPI_M_write(0xA1); //Segment remap
    SPI_M_write(0xDA); // Common pads hardware: alternative
    SPI_M_write(0x12); // Set higher column start adress
    SPI_M_write(0xC8); // Common output scan direction
    SPI_M_write(0xA8); // Set mux ration mode: 63
    SPI_M_write(0x3f); // 63
    SPI_M_write(0xD5); // Display divide ratio/freq. mode;
    SPI_M_write(0x80); //
    SPI_M_write(0x81); // Contrast control
    SPI_M_write(0x7F); //
    SPI_M_write(0x69); // Set pre-charge period
    SPI_M_write(0x21); // Set column adress
    SPI_M_write(0x20); // Set memory adressing mode
    SPI_M_write(0x02); //
    SPI_M_write(0xDB); // VCOM deselect level mode 
    SPI_M_write(0x30); //
    SPI_M_write(0x00);
    SPI_M_write(0xA4); // Follow RAM mode
    SPI_M_write(0xA6); // Set normal display
    SPI_M_write(OLED_ON); //set display on*/
}

void oled_reset(){
    SPI_select_slave(OLED_CMD);
    SPI_M_write(0xA6); //reset OLED
}

void oled_home(); //set pos to 0,0

void oled_goto_line(uint8_t line);

void oled_clear_line(uint8_t line);

void oled_set_pos(uint8_t row, uint8_t col);

void oled_write_data(char c){ //something something volatile
    SPI_select_slave(OLED_DATA);
    while(1){
        SPI_M_write(0b11111111);
    }
}
void oled_print(char* string);

void oled_set_brightness(uint8_t level);