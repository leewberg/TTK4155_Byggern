#pragma once
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

enum SLAVES {CAN, AVR, OLED_CMD, OLED_DATA};

void SPI_M_init(void);

void SPI_M_write(char cData);

char SPI_M_read(void);

void SPI_select_slave(enum SLAVES slave);