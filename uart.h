#pragma once
#include <avr/io.h>
#include <stdio.h>

void uart_init(unsigned int ubrr);

int uart_transmit_f(char data, FILE *file);
int uart_reciever_f(FILE *file);

int uart_transmit(unsigned char data);
int uart_reciever();
