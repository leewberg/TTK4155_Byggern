#include "consts.h"
#include <avr/io.h>
#include "uart.h"
#include "SRAM.h"
#include "unit_tests/latch.h"
#include "unit_tests/SRAM.h"
#include "unit_tests/uart.h"
#include "adc.h"
#include "SPI.h"

//picocom command to read output from computer
//picocom -b 9600 -p 2 -r -l /dev/ttyS0
//git command for ask credentials on public computer:
//GIT_ASKPASS= git push https://github.com/leewberg/TTK4155_Byggern.git

int main(){
    uart_init(MYUBRR);
    SRAM_init();
    SPI_M_init();
    oled_init();
    oled_write_data('a');

    //const volatile ADC_DATA* adc_data = adc_init();
    //adc_test();
    return 0;
}
