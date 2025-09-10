#pragma once
#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

#define BASE_ADRESS 0x1800 //SRAM start adress. alt: 0x1400

void SRAM_init();

void SRAM_write(uint8_t data, uint16_t adr);

uint8_t SRAM_read(uint16_t addr);