#include <avr/io.h>
#include "uart.h"
#include "SRAM.h"
#include "unit_tests/latch.h"
#include "unit_tests/SRAM.h"
#include "unit_tests/uart.h"
#include "consts.h"

//picocom command to read output from computer
//picocom -b 9600 -p 2 -r -l /dev/ttyS0

int main(){
    uart_init(MYUBRR);
    //SRAM_init();
    latch_test();
        //printf("Hello World!");
    return 0;
}
