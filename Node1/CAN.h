#pragma once
#include "SPI.h"
#include "consts.h"
#include "mcp2515.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

// Send-buffer 0
#define MCP_TXB0SIDH 0x31

// Send-buffer 1
#define MCP_TXB1SIDH 0x41

// Send-buffer 2
#define MCP_TXB2SIDH 0x51

// Motta-buffer 0
#define MCP_RXB0SIDH 0x61

// Motta-buffer 1
#define MCP_RXB1SIDH 0x71

// PB0: interrupt pin for CAN

int can_init(void);

volatile uint8_t can_read(uint8_t adress);

void can_write(uint8_t adress, uint8_t data);

void can_rts(int buffer_number);

volatile char can_rs();

void can_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);

void can_reset();

void can_set_mode(uint8_t mode);
