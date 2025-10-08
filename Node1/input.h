#pragma once
#include "adc.h"
#include <avr/io.h>

typedef struct {
	uint8_t slider;		// 0-255
	uint8_t joy_button; // info in LSB rest is useless?
	POS joy_pos;   // same as raw_data from adc. might do the same processing if
				   // we decide to use it
	POS touch_pos; // ditto
	uint8_t right_btns; // total of 6 buttons, the two MSBs do nothing
	uint8_t left_btns;	// total of 7 buttons, MSB does nothing
	uint8_t nav_switch; // bit 0: up, bit 1: down, bit 2: left, bit 3: right,
						// bit 4: button
} INPUT_DATA;

// TODO move this to a different file later
typedef struct {
	uint8_t leds;
} LED_DATA;

// run intterupts on this, but only setting a flag and doing the work in main

volatile INPUT_DATA *input_init();

void input_read(volatile INPUT_DATA *data);
void set_leds(volatile LED_DATA *data);
void input_print(volatile const INPUT_DATA *data);
