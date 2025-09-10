#include "../uart.h"

void uart_test_reciever(void){
    if (uart_reciever() == 'a'){
            uart_transmit('b');
        }
    _delay_ms(100);
}

void uart_test_transmitter(void){
    uart_transmit('a');
}
