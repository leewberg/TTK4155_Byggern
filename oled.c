#pragma once
#include "oled.h"
#include "fonts.h"
#include "SPI.h"

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
	SPI_deselect();
	oled_reset();
	oled_home();
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
    SPI_M_write(0xA6); //reset OLED. yeah that does not work
	SPI_deselect();
	for (uint8_t i = 0; i < 8; i++){
		oled_clear_line(i);
	}

}

void oled_home(){ oled_set_pos(0,0); }

void oled_goto_line(uint8_t line){
	SPI_select_slave(OLED_CMD);
	//some bitmagic here: 0xB0 is the prefix for the command, the lower 4 bits are the line number and they are masked with & 0x0F
	SPI_M_write(0xB0 | (line & 0x0F)); 
	SPI_deselect();
}
void oled_goto_col(uint8_t col){
	SPI_select_slave(OLED_CMD);
	// some more bitmagic: we have to split the 8 bit column number into two 4 bit parts and send them with different commands
	SPI_M_write(0x00 | (col & 0x0F)); // lower 4 bits
	SPI_M_write(0x10 | ((col >> 4) & 0x0F)); // upper 4 bits
	SPI_deselect();
}

void oled_clear_line(uint8_t line){
	oled_set_pos(line, 0);
	for (uint8_t i = 0; i < 128; i++){
		oled_write_data(0x00);
	}
}

void oled_set_pos(uint8_t line, uint8_t col){
	oled_goto_line(line);
	oled_goto_col(col);
}

void oled_write_data(const char c){ //something something volatile
	// test out if the constant selecting and deselecting is required
    SPI_select_slave(OLED_DATA);
	SPI_M_write(c);
	SPI_deselect();
}

void oled_print(const char* string){
	while (*string){
		char c = *string++; //returns current char from string and increments the pointer
		for (uint8_t i = 0; i < 8; i++){
			oled_write_data(pgm_read_byte(&font8[c-32][i])); //reads the byte from PROGMEM and sends it to the oled
		}
	}


void oled_write_cmd(char c){
    SPI_select_slave(OLED_CMD);
    SPI_M_write(c);
    PORTB |= (1<<PB3);
}

void oled_set_brightness(uint8_t level){
	SPI_select_slave(OLED_CMD);
	SPI_M_write(0x81); //Contrast control
	SPI_M_write(level); //level 0-255
	SPI_deselect();
}
