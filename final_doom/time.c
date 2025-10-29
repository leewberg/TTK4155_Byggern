#include "sam3x8e.h"
#include "constants.h"
#include "time.h"

volatile uint32_t systick_ms = 0;

void time_init(void) {
	SysTick_Config(F_CPU / 1000); // 1ms tick
}

void SysTick_Handler(void) { // this is an interrupt handler. make TODO: make sure its max periority
	systick_ms++;
}

void delay_ms(uint32_t ms) {
	uint32_t target = systick_ms + ms;
	while (systick_ms < target){}
}
