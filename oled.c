#pragma once
#include "oled.h"
#include "SPI.h"
#include <util/delay.h>

void oled_init(){
    oled_write_cmd(0xae); //off

    oled_write_cmd(0xd5); //set display clock divide / 
    oled_write_cmd(0x80);
    
    oled_write_cmd(0xa8); //mux ratio
    oled_write_cmd(0x3f);

    oled_write_cmd(0xd3); //display offset
    oled_write_cmd(0x00);

    oled_write_cmd(0x40); //display start line at 0

    oled_write_cmd(0x8d); //charge pump
    oled_write_cmd(0x14); //enable charge pump

    oled_write_cmd(0x20); //set memory adressing mode
    oled_write_cmd(0x02); //page mode

    oled_write_cmd(0xa1); //set segment re-map

    oled_write_cmd(0xc8); //set com output scan direction

    oled_write_cmd(0xda); //set com pins hardware config
    oled_write_cmd(0x12);

    oled_write_cmd(0x81); //set contrast level
    oled_write_cmd(0x7f);

    oled_write_cmd(0xd9); //set pre-charge period
    oled_write_cmd(0xf1);

    oled_write_cmd(0xdb); //set comh deselect level
    oled_write_cmd(0x40);

    oled_write_cmd(0xa4); //resume ram content display

    oled_write_cmd(0xa6); //normal display (not inverted)

    oled_write_cmd(0xaf); //display on
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
    oled_write_cmd(c);
    PORTB |= (1<<PB3);
}

void oled_write_cmd(char c){
    SPI_select_slave(OLED_CMD);
    SPI_M_write(c);
    PORTB |= (1<<PB3);
}

void oled_clear(void){
    for(uint8_t page = 0; page < 8; page++){
        oled_write_cmd(0xb0 + page);
        oled_write_cmd(0x00);
        oled_write_cmd(0x10);

        for(uint8_t col = 0; col < 128; col++){
            oled_write_data(0x00);
        }
    }
}
void oled_print(char* string);

void oled_set_brightness(uint8_t level);
