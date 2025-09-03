#pragma once
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void uart_init(unsigned int ubrr);

int uart_transmit(unsigned char data, FILE* file);
int uart_reciever(FILE* file);

void uart_test_reciever(void);

void uart_test_transmitter(void);