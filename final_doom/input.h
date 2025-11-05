#ifndef _input_h
#define _input_h
#include <stdbool.h>
#include <stdint.h>

typedef struct {
	bool up;
	bool down;
	bool left;
	bool right;
	bool fire;
	bool reset;
	bool CTS; //Node 1 is ready to receive data
	volatile uint32_t start_time; //time since the last program fail in ms
	uint8_t joy_x; //magnitude: 0-100
	uint8_t joy_y;
	uint8_t slider;
} Inputs;

extern volatile Inputs* input_data;

void input_setup();
void update_inputs(uint8_t new_state, uint8_t joy_x, uint8_t joy_y, uint8_t slider);

#endif
