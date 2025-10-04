#pragma once
#include "adc.h"
#include "consts.h"
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

int JOY_X_OFFSET;
int JOY_Y_OFFSET;

static volatile char *adc = (char *)ADC_ADDR;
static volatile ADC_DATA *adc_data; // static so only stuff in here can access
									// it, volatile cause of the ISR

// private functions
void joystick_norm(volatile ADC_DATA *adc_data);
void adc_get(volatile ADC_DATA *adc_data);
void calibrate_joystick(volatile ADC_DATA *adc_data);

int adc_digital_to_angle(volatile const int D, const int D_min, const int D_mid,
						 const int D_max) {
	// this could be a more general util for renormalizing values
	if (D >= D_mid) {
		return ((D - D_mid) * 100) / (D_max - D_mid);
	} else {
		return ((D - D_mid) * 100) / (D_mid - D_min);
	}
}

enum DIRECTION get_direction(volatile const POS *joy) {
	if (joy->y > JOY_DEADZONE) {
		return UP;
	} else if (joy->y < -JOY_DEADZONE) {
		return DOWN;
	} else if (joy->x > JOY_DEADZONE) {
		return RIGHT;
	} else if (joy->x < -JOY_DEADZONE) {
		return LEFT;
	} else {
		return NEUTRAL;
	}
}

void adc_get(volatile ADC_DATA *adc_data) {
	adc[0] = 0;
	_delay_us(CONVERSION_TIME);
	adc_data->touch.x = (uint8_t)adc[0];
	adc_data->joy.y = (uint8_t)adc[0];
	adc_data->joy.x = (uint8_t)adc[0];
	adc_data->touch.y = (uint8_t)adc[0];
}

void joystick_norm(volatile ADC_DATA *adc_data) {
	adc_data->joy.x += JOY_X_OFFSET;
	adc_data->joy.y += JOY_Y_OFFSET;
	adc_data->joy.x =
		adc_digital_to_angle(adc_data->joy.x, Dx_min, Dx_mid, Dx_max);
	adc_data->joy.y =
		adc_digital_to_angle(adc_data->joy.y, Dy_min, Dy_mid, Dy_max);
}

void calibrate_joystick(volatile ADC_DATA *adc_data) {
	adc_get(adc_data);
	// center the joystick values around 256/2 = 128
	JOY_X_OFFSET = 128 - adc_data->joy.x;
	JOY_Y_OFFSET = 128 - adc_data->joy.y;
}

void adc_print(volatile const ADC_DATA *adc_data) {
	printf(
		"Touchpad x: %4d Toucpad y: %4d Joystick x: %4d Joystick y: %4d  \n\r",
		adc_data->touch.x, adc_data->touch.y, adc_data->joy.x, adc_data->joy.y);
}

void adc_test() {
	while (1) {
		adc_print(adc_data);
	}
}

ISR(TIMER0_OVF_vect) {
	adc_get(adc_data);
	joystick_norm(adc_data);
	adc_data->dir = get_direction(&adc_data->joy);
}

volatile const ADC_DATA *adc_init(void) {
	// initiate the CTC mode for creating a clock signal for the ADC
	DDRD |= (1 << PD5); // enable output on pin D5
	TCCR1A = 0;			// reset configuration
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12);
	TCCR1A |= (1 << COM1A0);
	OCR1A = OCR1A_VAL;
	TCCR1B |= (1 << CS10);

	static ADC_DATA my_data; // static, so we don't make multiple copies
	adc_data = &my_data;

	JOY_X_OFFSET = 0;
	JOY_Y_OFFSET = 0;
	calibrate_joystick(adc_data);

	// set up timer0 for interrupt
	TCCR0 = (1 << CS01) | (1 << CS00); // Prescaler 64
	TIMSK |= (1 << TOIE0);
	sei(); // Enable global interrupts

	return adc_data;
}
