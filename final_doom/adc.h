#pragma once

#include "sam.h"
#include "constants.h"
#include <stdint.h>
#include "input.h"

#define LOWTRESHOLD 1240 //0.5V



void adc_init(void);

void ADC_Handler(void);

void print_trigger(void);
