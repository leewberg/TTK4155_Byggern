#include <avr/io.h>
#include "uart.h"

#define FOSC 4915200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD -1 
//picocom command to read output from computer
//picocom -b 9600 -p 2 -r -l /dev/ttyS0

int main(){
    uart_init(MYUBRR);
    printf("Hello World!");
    return 0;
}
