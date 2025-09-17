#pragma once
#include "consts.h"
#include <avr/io.h>

int JOY_X_OFFSET;
int JOY_Y_OFFSET;
enum DIRECTION {UP, DOWN, LEFT, RIGHT, NEUTRAL};
enum DIRECTION get_direction(int* adc_out);

void adc_init(void);
int adc_digital_to_angle(int D, const int D_min, const int D_mid, const int D_max);
void adc_test(void);
void joystick_norm(int* adc_out);

int* adc_get(void);
void adc_print(int* adc_out);
void calibrate_joystick(void);
