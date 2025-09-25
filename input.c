#include "consts.h"
#include <avr/io.h>
#include "input.h"
#include "SPI.h"
#include <util/delay.h>

volatile INPUT_DATA* input_init(){
	//init adc
	//init spi
	SPI_M_init();
	//return pointer to static data struct
	volatile static INPUT_DATA data;
	return &data;
}

void input_read(volatile INPUT_DATA* data){

	// touch pad
	SPI_select_slave(AVR_s);
	SPI_M_write(0x01); //send read command
	_delay_us(40);
	data->touch_pos.x = (uint8_t) SPI_M_read(); //read x
	_delay_us(2);
	data->touch_pos.y = (uint8_t) SPI_M_read(); //read y
	SPI_deselect();

	// touch slider
	SPI_select_slave(AVR_s);
	SPI_M_write(0x02); //send read command
	_delay_us(40);
	data->slider = (uint8_t) SPI_M_read(); //read x
	SPI_deselect();

	// joystick
	SPI_select_slave(AVR_s);
	SPI_M_write(0x03); //send read command
	_delay_us(40);
	data->joy_pos.x = (uint8_t) SPI_M_read(); //read x
	_delay_us(2);
	data->joy_pos.y = (uint8_t) SPI_M_read(); //read y
	_delay_us(2);
	data->joy_button = (uint8_t) SPI_M_read(); //read joystick button
	SPI_deselect();

	// buttons
	SPI_select_slave(AVR_s);
	SPI_M_write(0x04); //send read command
	_delay_us(40);
	data->right_btns = (uint8_t) SPI_M_read(); //read right buttons
	_delay_us(2);
	data->left_btns = (uint8_t) SPI_M_read(); //read left buttons
	_delay_us(2);
	data->nav_switch = (uint8_t) SPI_M_read(); //read nav switch
	SPI_deselect();
}

void set_leds(volatile LED_DATA* data){
	for (int i = 0; i < 6; i++){
		SPI_select_slave(AVR_s);
		SPI_M_write(0x05);
		_delay_us(40);
		SPI_M_write(i);
		_delay_us(2);
		SPI_M_write(data->leds & (1 << i));
		SPI_deselect();
	}
}

void input_print(volatile const INPUT_DATA* data){
	printf("Touch: x: %3d, y: %3d ", data->touch_pos.x, data->touch_pos.y);
	printf("Slider: %3d ", data->slider);
	printf("Joystick: x: %3d, y: %3d, button: %d ", data->joy_pos.x, data->joy_pos.y, data->joy_button & 0x01);
	printf("Right buttons: %02X ", data->right_btns);
	printf("Left buttons: %02X ", data->left_btns);
	printf("Nav switch: %02X\n\r", data->nav_switch);
}
