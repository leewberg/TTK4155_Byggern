#include "output.h"
#include "input.h"
#include <stdbool.h>

CanInit canInit;

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


void can_transmit() {
    while (!can_rx(&recv_msg) || recv_msg.id != 2);
	
	// sends the display buffer over CAN
	update_inputs(recv_msg.byte[0], recv_msg.byte[1], recv_msg.byte[2], recv_msg.byte[3]);
}
