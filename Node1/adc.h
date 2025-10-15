#pragma once
#include "consts.h"
#include <avr/io.h>

typedef enum { UP, DOWN, LEFT, RIGHT, NEUTRAL } DIRECTION;

typedef struct {
	int x;
	int y;
} POS;

typedef struct {
	POS joy;
	POS touch;
	DIRECTION dir;
} ADC_DATA;

volatile const ADC_DATA *adc_init(void);
void adc_test(void);

void adc_print(volatile const ADC_DATA
				   *adc_data); // this one should take inn adc_data, becuase its
							   // a util to be used outside adc.c

int adc_digital_to_angle(volatile const int D, const int D_min, const int D_mid,
						 const int D_max);
