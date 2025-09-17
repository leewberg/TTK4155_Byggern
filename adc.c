#pragma once
#include "adc.h"
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include "consts.h"

void adc_test(void){
	while (1){
		int* arr = adc_get();
		adc_print(arr);
		free(arr);
	}
}

enum DIRECTION get_direction(int* adc_out){
	if(adc_out[1] > JOY_DEADZONE + 128){
		return UP;
	}
	else if(adc_out[1] < 128 - JOY_DEADZONE){
		return DOWN;
	}
	else if(adc_out[0] > JOY_DEADZONE + 128){
		return RIGHT;
	}
	else if(adc_out[0] < 128 - JOY_DEADZONE){
		return LEFT;
	}
	else{
		return NEUTRAL;
	}
}

int* adc_get(void){
	volatile char *adc = (char *)ADC_ADDR;
	volatile uint8_t touch_x = 0;
	volatile uint8_t joy_y = 0;
	volatile uint8_t joy_x = 0;
	volatile uint8_t touch_y = 0;
	// conversion initiates with read
	adc[0] = 0;
	_delay_us(CONVERSION_TIME*1000000);
	touch_x = adc[0];
	joy_y = adc[0];
	joy_x = adc[0];
	touch_y = adc[0];

	// format output
	int* adc_out = malloc(4 * sizeof(int));
	adc_out[2] = touch_x;
	adc_out[1] = joy_y + JOY_Y_OFFSET;
	adc_out[0] = joy_x + JOY_X_OFFSET;
	adc_out[3] = touch_y;
	return adc_out;
}

void calibrate_joystick(void){
	int* adc_out = adc_get();
	// center the joystick values around 256/2 = 128
	JOY_X_OFFSET = 128 - adc_out[0];
	JOY_Y_OFFSET = 128 - adc_out[1];
	free(adc_out);
}

void adc_print(int* adc_out){
	printf("Touchpad x: %4d Toucpad y: %4d Joystick x: %4d Joystick y: %4d  \n\r", adc_out[2], adc_out[3], adc_out[0], adc_out[1]);
}


void adc_init(void){
	// initiate the CTC mode for creating a clock signal for the ADC
    DDRD |= (1<<PD5);
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |=(1<<WGM12);
    TCCR1A |= (1<<COM1A0);
    OCR1A = OCR1A_VAL;
    TCCR1B |= (1<<CS10);

	JOY_X_OFFSET = 0;
	JOY_Y_OFFSET = 0;
	calibrate_joystick();
}
