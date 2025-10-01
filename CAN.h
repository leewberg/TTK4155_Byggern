#pragma once
#include <avr/io.h>
#include "consts.h"
#include "mcp2515.h"
#include "SPI.h"
#include <stdio.h>

//PB0: interrupt pin for CAN

void can_init(void);

volatile uint8_t can_read(uint8_t adress);

void can_write(uint8_t adress, uint8_t data);

void can_rts(int buffer_number);

volatile char can_rs();

void can_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);

void can_reset();

void can_set_mode (uint8_t mode);