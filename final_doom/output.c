#include "output.h"
#include "can.h"
#include "display.h"
#include "input.h"
#include <stdbool.h>



static CanInit canInit;
static CanMsg send_msg;
static CanMsg recv_msg;

void output_init() {
	// Initialize CAN module
	canInit.brp = 5;
	canInit.phase1 = 6;                // Phase 1 segment
	canInit.phase2 = 3;                // Phase 2 segment
	canInit.propag = 1;                // Propagation time segment
	canInit.sjw = 3;                   // Synchronization jump width
	canInit.smp = 0;                   // Sampling mode
	
	send_msg.id = 0x000;
	send_msg.length = 8;

	recv_msg.id = 0x000;
	recv_msg.length = 4;

	can_init(canInit, 1);
}

// busy waits if CTS not yet set
void can_transmit() {
	while (!input_data->CTS) {}
		
	// sends the entire display buffer over CAN
	for (uint8_t i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT) / 64; i++) {
		for (uint8_t j = 0; j < 8; j++) {
			send_msg.byte[j] = display_buf[i * 8 + j];
		}

		send_msg.id = i;
		can_tx(send_msg);
	}
	input_data->CTS = false;
}
