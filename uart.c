#include <avr/io.h>
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

    fdevopen(uart_transmit, uart_reciever);
}

int uart_transmit(unsigned char data, FILE * file){ //as fdevopen() takes in two functions with return-type int, and with parametre-types of char and FILE*, the transmit 
    /*waiting for empty transmit buffer*/
    while (!(UCSR0A & (1<<UDRE0)));
    /*geting and returning recieved data from buffer*/
    UDR0 = data;
    PORTB = 1;

    return 0;
}

int uart_reciever(FILE * file){
    /*waiting for data to be recieved*/
    while( !(UCSR0A & (1<<RXC0)));
    return UDR0;
}

/*void uart_test_reciever(void){
    if (uart_reciever() == 'a'){
            uart_transmit('b');
        }
    _delay_ms(100);
}*/

/*void uart_test_transmitter(void){
    uart_transmit('a');
}*/