#include "CAN.h"

int can_init(void) {
	SPI_M_init();

	// set config mode
	can_set_mode(MODE_CONFIG);

	can_reset();

	_delay_ms(1);

	uint8_t value = can_read (MCP_CANSTAT);
	if (( value & MODE_MASK ) != MODE_CONFIG ) {
		printf ( " MCP2515 is NOT in configuration mode after reset !\n " ) ;
		return 1;
	}
	can_write(MCP_CNF1, 0b00001001);
	can_write(MCP_CNF2, 0b10010000);
	can_write(MCP_CNF3, 0b00000001);
	//if we want to use the interrupt generated when mpc2515 exits sleep-mode, the data should be 0b01000101
	return 0;
}

volatile uint8_t can_read(uint8_t adress) {
	SPI_select_slave(CAN);
	SPI_M_write(MCP_READ);
	SPI_M_write(adress);
	uint8_t data = SPI_M_read();
	SPI_deselect();
	return data;
}

void can_write(uint8_t adress, uint8_t data) {
	SPI_select_slave(CAN);
	SPI_M_write(MCP_WRITE);
	SPI_M_write(adress);
	SPI_M_write(data);
	SPI_deselect();
}

void can_rts(int buffer_number) {
	/*buffer_number = 0: TX0
	buffer_number = 1: TX1
	buffer_number = 3: TX2
	buffer_number = 6: ALL
	*/
	uint8_t rts_cmd;
	switch (buffer_number){
		case 0:	rts_cmd = MCP_RTS_TX0; break;
		case 1:	rts_cmd = MCP_RTS_TX1; break;
		case 3:	rts_cmd = MCP_RTS_TX2; break;
		case 6:	rts_cmd = MCP_RTS_ALL; break;
		default: return;
	}
	SPI_select_slave(CAN);
	SPI_M_write(rts_cmd);
	SPI_deselect();
}

volatile char can_rs() {
	SPI_select_slave(CAN);
	SPI_M_write(MCP_READ_STATUS);
	char data = SPI_M_read();
	SPI_deselect();
	return data;
}

void can_bit_modify(uint8_t adress, uint8_t mask, uint8_t data) {
	SPI_select_slave(CAN);
	SPI_M_write(MCP_BITMOD);
	SPI_M_write(adress);
	SPI_M_write(mask);
	SPI_M_write(data);

	SPI_deselect();
}

void can_reset() {
	SPI_select_slave(CAN);
	SPI_M_write(MCP_RESET);
	SPI_deselect();
}

void can_set_mode(uint8_t mode) {
	can_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}
