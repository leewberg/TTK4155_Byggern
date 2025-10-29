#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "sam.h"
#include "constants.h"

void encoder_init(void);

uint32_t encoder_read();
