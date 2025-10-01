#include <avr/io.h>
#include "consts.h"
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"

void uart_init(unsigned int ubrr){
    /*setting baud-rate*/
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /*enabeling reciever and transmitter*/
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /*setting frame format: 8data, 2stop bit*/
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

    fdevopen(uart_transmit_f, uart_reciever_f);
}

// idk why they are here, but they are needed for fdevopen to work
int uart_transmit_f(char data, FILE * file){
    /*waiting for empty transmit buffer*/
    while (!(UCSR0A & (1<<UDRE0)));
    /*geting and returning recieved data from buffer*/
    UDR0 = data;

    return 0;
}

int uart_reciever_f(FILE * file){
    /*waiting for data to be recieved*/
    while( !(UCSR0A & (1<<RXC0)));
    return UDR0;
}

int uart_transmit(unsigned char data){
    /*waiting for empty transmit buffer*/
    while (!(UCSR0A & (1<<UDRE0)));
    /*geting and returning recieved data from buffer*/
    UDR0 = data;

    return 0;
}

int uart_reciever(){
    /*waiting for data to be recieved*/
    while( !(UCSR0A & (1<<RXC0)));
    return UDR0;
}

