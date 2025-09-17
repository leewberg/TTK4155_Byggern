#pragma once
#include "consts.h"
#include <avr/io.h>

enum DIRECTION {UP, DOWN, LEFT, RIGHT, NEUTRAL};

typedef struct {
	int x;
	int y;
} POS;

typedef struct {
	POS joy;
	POS touch;
	enum DIRECTION dir;
} ADC_DATA;


ADC_DATA* adc_init(void); // set up interrupts here
void adc_test(void);

void adc_print(const ADC_DATA* adc_data); // this one should take inn adc_data, becuase its a util to be used outside adc.c
