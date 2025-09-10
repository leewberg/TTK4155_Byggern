#pragma once
#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

#define BASE_ADRESS 0x1400 //SRAM start adress

void SRAM_init();

void SRAM_write(uint8_t data, uint16_t adr);

uint8_t SRAM_read(uint16_t addr);