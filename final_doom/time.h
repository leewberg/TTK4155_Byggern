#pragma once
#include <stdint.h>

extern volatile uint32_t systick_ms; //updated via SysTick_Handler interrupt

void time_init(void);

void delay_ms(uint32_t ms);
