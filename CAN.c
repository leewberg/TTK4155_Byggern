#include "CAN.h"

void can_init(void){
    SPI_M_init();

    //set config mode
    can_set_mode(MODE_CONFIG);

    can_reset();

    _delay_ms(1);
}

volatile uint8_t can_read(uint8_t adress){
    SPI_select_slave(CAN);
    SPI_M_write(MCP_READ);
    SPI_M_write(adress);
    uint8_t data = SPI_M_read();
    SPI_deselect();
    return data;
}

void can_write(uint8_t adress, uint8_t data){
    SPI_select_slave(CAN);
    SPI_M_write(MCP_WRITE);
    SPI_M_write(adress);
    SPI_M_write(data);
    SPI_deselect();
}

void can_rts(int buffer_number){
    /*buffer_number = 0: TX0
    buffer_number = 1: TX1
    buffer_number = 3: TX2
    buffer_number = 6: ALL
    */
    SPI_select_slave(CAN);
    SPI_M_write(MCP_RTS_ALL-(6-buffer_number));
    SPI_deselect();
}

volatile char can_rs(){
    SPI_select_slave(CAN);
    SPI_M_write(MCP_READ_STATUS);
    char data = SPI_M_read();
    SPI_deselect();
    return data;
}

void can_bit_modify(uint8_t adress, uint8_t mask, uint8_t data){
    SPI_select_slave(CAN);
    SPI_M_write(MCP_BITMOD);
    SPI_M_write(mask);
    SPI_M_write(data);

    SPI_deselect();
}

void can_reset(){
    SPI_select_slave(CAN);
    SPI_M_write(MCP_RESET);
    SPI_deselect();
}

void can_set_mode (uint8_t mode){
    can_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}