#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "sam.h"
#include "const.h"
#include "pwm.h"
#include "encoder.h"
#include "input.h"
//2813

void motor_init();

void motor_control();
