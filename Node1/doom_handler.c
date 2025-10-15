#include "doom_handler.h"
#include "SRAM.h"

volatile INPUT_DATA* data;
// volatile uint8_t* FRAME_BUFFER;
can_frame msg;

void doom_init() {
	SPI_M_init();
	oled_init(); // the transfer uses page mode
	oled_reset();
	SRAM_init();

	can_init();
	can_set_mode(MODE_NORMAL);
	printf("mode: %x\r\n", can_read(MCP_CANSTAT));

	data = input_init();
	// FRAME_BUFFER = (uint8_t*)BASE_ADRESS;

}

void fetch_and_send_inputs() {
	input_read(data);
	msg.id = 0x02;
	msg.length = 3;
	msg.data[0] = 0;
	
	msg.data[0] |= (data->joy_pos.y > 128 + JOY_DEADZONE); // up
	msg.data[0] |= (data->joy_pos.y < 128 - JOY_DEADZONE) << 1; // down
	msg.data[0] |= (data->joy_pos.x < 128 - JOY_DEADZONE) << 2; // left
	msg.data[0] |= (data->joy_pos.x > 128 + JOY_DEADZONE) << 3; // right
	msg.data[0] |= (data->right_btns & 0b100000) >> 1; // fire button is rb6
	msg.data[0] |= (data->left_btns & 0b1) << 5; // reset button is lb1
	
	msg.data[1] = data->joy_x_abs;
	msg.data[2] = data->joy_y_abs;

	CAN_send(&msg);
}

void send_ready_frame() {
	msg.id = 2;
	msg.length = 1;
	msg.data[0] = 0xFF;
	CAN_send(&msg);
}

// waiting for every frame
void fetch_frame() {
	// this works as long as we enter before the transmission starts
	// and as long as we are processing faster than the transmission
		
	for (uint8_t i = 0; i < SCREEN_HEIGHT / 8; i++) { // pages
		oled_set_pos(i, 0);
		for (uint8_t j = 0; j < SCREEN_WIDTH/8; j++) { // 8 byte chunks
			while(!CAN_receive(&msg)) {} // wait for there to be a message
			// if (recv_frame.id != i) {
			// 	printf("Frame ID mismatch \r\n");
			// }
				
			for (uint8_t d = 0; d < 8; d++) {
				oled_write_data(msg.data[d]);
			}
			// send_ready_frame();
		}
	}
}

void doom_loop() {
	send_ready_frame();
	fetch_frame();
	fetch_and_send_inputs();
}
