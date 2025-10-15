#include "CAN.h"

int can_init(void) {
	SPI_M_init();

	can_reset();

	// set config mode
	can_set_mode(MODE_CONFIG);

	_delay_ms(1);

	uint8_t value = can_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_CONFIG) {
		printf(" MCP2515 is NOT in configuration mode after reset !\n ");
		return 1;
	}
	can_write(MCP_CNF1, 0b00000000);
	can_write(MCP_CNF2, 0b10011000);
	can_write(MCP_CNF3, 0b00000001);
	// if we want to use the interrupt generated when mpc2515 exits sleep-mode,
	// the data should be 0b01000101
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
	switch (buffer_number) {
	case 0:
		rts_cmd = MCP_RTS_TX0;
		break;
	case 1:
		rts_cmd = MCP_RTS_TX1;
		break;
	case 3:
		rts_cmd = MCP_RTS_TX2;
		break;
	case 6:
		rts_cmd = MCP_RTS_ALL;
		break;
	default:
		return;
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

void CAN_send(const can_frame *message) {
	can_write(TXB0SIDH, (message->id >> 3));
	can_write(TXB0SIDL, (message->id << 5));
	can_write(TXB0DLC, message->length);
	for (int i = 0; i < message->length; i++) {
		can_write(TXB0DLC + 1 + i, message->data[i]);
	}
	can_rts(0);
	uint8_t reg = can_read(TXB0CTRL);
	if (reg & (1 << TXREQ)) { // if message message still pending
		if (reg & (1 << TXERR)) { // error detected
			printf("CAN_ERROR: msg error detected \n\r");
		} else if (reg & (1 << MLOA)) { // message lost arbitration
			printf("CAN_ERROR: MSG lost\n\r");
		}
	}
}

bool CAN_receive(can_frame *message) {
	uint8_t status = can_rs();		

	if (status & 0b1 || status & 0b10) { //message in buffer 0 or 1
		SPI_select_slave(CAN);

		// no need for generic read: MCP has this opcode specifically for reading RX0
		if (status & 0b1) {			//message in buffer 0
			SPI_M_write(MCP_READ_RX0); 
		} else {
			SPI_M_write(MCP_READ_RX1);
		}
		uint8_t sidh = SPI_M_read();
		uint8_t sidl = SPI_M_read();
		SPI_M_read(); //dummy read of EID8
		SPI_M_read();
		message->id = (sidh << 3) | (sidl >> 5); //why does it have to be this way?
		message->length = SPI_M_read();
		for (int i = 0; i < message->length; i++) {
			message->data[i] = SPI_M_read();
		}
		SPI_deselect();
		return true;
	}
	return false;
}
