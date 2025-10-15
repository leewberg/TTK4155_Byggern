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
	uint8_t joy_x; //magnitude: 0-100
	uint8_t joy_y;
} Inputs;

extern Inputs* input_data;

void input_setup();
void update_inputs(uint8_t new_state, uint8_t joy_x, uint8_t joy_y);

#endif
