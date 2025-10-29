#pragma once
#include "input.h"
#include <stdlib.h>

Inputs* input_data;

void input_setup() {
	input_data = (Inputs *)malloc(sizeof(Inputs));
	input_data->up = false;
	input_data->down = false;
	input_data->left = false;
	input_data->right = false;
	input_data->fire = false;
	input_data->reset = false;
	input_data->CTS = false;
	input_data->start_time = 0;
	input_data->joy_x = 100;
	input_data->joy_y = 100;
}

void update_inputs(uint8_t new_state, uint8_t joy_x, uint8_t joy_y, uint8_t slider) {
	input_data->up = new_state & 0b00000001;
	input_data->down = new_state & 0b00000010;
	input_data->left = new_state & 0b00000100;
	input_data->right = new_state & 0b00001000;
	input_data->fire = new_state & 0b00010000;
	input_data->reset = new_state & 0b00100000;
	input_data->joy_x = joy_x;
	input_data->joy_y = joy_y;
	input_data->slider = slider;
	// here, set PIO for 10ms pulse
}
