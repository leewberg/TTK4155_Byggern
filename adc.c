#pragma once
#include "adc.h"
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include "consts.h"

int JOY_X_OFFSET;
int JOY_Y_OFFSET;

volatile char *adc = (char *)ADC_ADDR;
volatile ADC_DATA* adc_data;

//private functions
void joystick_norm();
void adc_update();
void adc_get(void);
void calibrate_joystick(void);

int adc_digital_to_angle(const volatile int D, const int D_min, const int D_mid, const int D_max){
    if(D >= D_mid){
        return ((D - D_mid) * 100) / (D_max - D_mid);
    } else {
        return ((D - D_mid) * 100) / (D_mid - D_min);
    }
}

void adc_test(void){
	while (1){
		adc_update();
		adc_print(adc_data);
	}
}

enum DIRECTION get_direction(const volatile POS* joy){
	if(joy->y > JOY_DEADZONE){
		return UP;
	}
	else if(joy->y <  -JOY_DEADZONE){
		return DOWN;
	}
	else if(joy->x > JOY_DEADZONE){
		return RIGHT;
	}
	else if(joy->x <  -JOY_DEADZONE){
		return LEFT;
	}
	else{
		return NEUTRAL;
	}
}

void adc_get(void){
	adc[0] = 0;
	_delay_us(CONVERSION_TIME*1000000);
	adc_data->touch.x = adc[0];
	adc_data->joy.y = adc[0];
	adc_data->joy.x = adc[0];
	adc_data->touch.y = adc[0];
}

void adc_update(){
	adc_get();
	joystick_norm();
	adc_data->dir = get_direction(&adc_data->joy);
}

void joystick_norm(){
	adc_data->joy.x += JOY_X_OFFSET;
	adc_data->joy.y += JOY_Y_OFFSET;
    adc_data->joy.x = adc_digital_to_angle(adc_data->joy.x, Dx_min, Dx_mid, Dx_max);
    adc_data->joy.y = adc_digital_to_angle(adc_data->joy.y, Dy_min, Dy_mid, Dy_max);
}

void calibrate_joystick(void){
	adc_get();
	// center the joystick values around 256/2 = 128
	JOY_X_OFFSET = 128 - adc_data->joy.x;
	JOY_Y_OFFSET = 128 - adc_data->joy.y;
}

void adc_print(const volatile ADC_DATA* adc_data){
	printf("Touchpad x: %4d Toucpad y: %4d Joystick x: %4d Joystick y: %4d  \n\r",
		adc_data->touch.x, adc_data->touch.y, adc_data->joy.x, adc_data->joy.y);
}


volatile ADC_DATA* adc_init(void){
	// initiate the CTC mode for creating a clock signal for the ADC
    DDRD |= (1<<PD5);
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |=(1<<WGM12);
    TCCR1A |= (1<<COM1A0);
    OCR1A = OCR1A_VAL;
    TCCR1B |= (1<<CS10);

	static ADC_DATA my_data;
	adc_data = &my_data;

	JOY_X_OFFSET = 0;
	JOY_Y_OFFSET = 0;
	calibrate_joystick();
	return adc_data;
}
