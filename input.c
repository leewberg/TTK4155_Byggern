#pragma once
#include <avr/io.h>
#include "input.h"
#include "SPI.h"
#include <util/delay.h>

const INPUT_DATA* input_init(){
	//init adc
	//init spi
	SPI_M_init();
	//return pointer to static data struct
	static INPUT_DATA data;
	return &data;
}

void input_read(INPUT_DATA* data){
	SPI_select_slave(AVR_s);

	// touch pad
	SPI_M_write(0x01); //send read command
	_delay_us(40);
	data->touch_pos.x = (uint8_t) SPI_M_read(); //read x
	data->touch_pos.y = (uint8_t) SPI_M_read(); //read y

	// touch slider
	SPI_M_write(0x02); //send read command
	_delay_us(40);
	data->slider = (uint8_t) SPI_M_read(); //read x

	// joystick
	SPI_M_write(0x03); //send read command
	_delay_us(40);
	data->joy_pos.x = (uint8_t) SPI_M_read(); //read x
	data->joy_pos.y = (uint8_t) SPI_M_read(); //read y
	data->joy_button = (uint8_t) SPI_M_read(); //read joystick button
	
	// buttons
	SPI_M_write(0x04); //send read command
	_delay_us(40);
	data->right_btns = (uint8_t) SPI_M_read(); //read right buttons
	data->left_btns = (uint8_t) SPI_M_read(); //read left buttons
	data->nav_switch = (uint8_t) SPI_M_read(); //read nav switch
}

void set_leds(LED_DATA* data){
	SPI_select_slave(AVR_s);
	for (int i = 0; i < 5; i++){
		SPI_M_write(0x05);
		_delay_us(20);
		SPI_M_write(i);
		SPI_M_write(data->leds & (1 << i));
	}
}

void input_print(INPUT_DATA* data){
	printf("Touch: x: %3d, y: %3d ", data->touch_pos.x, data->touch_pos.y);
	printf("Slider: %3d ", data->slider);
	printf("Joystick: x: %3d, y: %3d, button: %d ", data->joy_pos.x, data->joy_pos.y, data->joy_button & 0x01);
	printf("Right buttons: %02X ", data->right_btns);
	printf("Left buttons: %02X ", data->left_btns);
	printf("Nav switch: %02X\n\r", data->nav_switch);
}
