#pragma once
#include "consts.h"
#include <avr/io.h>

int JOY_X_OFFSET = 0;
int JOY_Y_OFFSET = 0;
enum DIRECTION {UP, DOWN, LEFT, RIGHT, NEUTRAL};
enum DIRECTION get_direction(int* adc_out);

void adc_init(void);

void adc_test(void);

int* adc_get(void);
void adc_print(int* adc_out);
void calibrate_joystick(void);
