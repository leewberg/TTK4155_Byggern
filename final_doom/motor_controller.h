#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "sam.h"
#include "const.h"
#include "pwm.h"
#include "encoder.h"
#include "input.h"
#include "time.h"
//2813

static uint32_t last_time;
static double integral;
static long double ref;

void motor_init();

void motor_control();
