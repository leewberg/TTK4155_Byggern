#include "consts.h"
#include <avr/io.h>
#include "uart.h"
#include "SRAM.h"
#include "unit_tests/latch.h"
#include "unit_tests/SRAM.h"
#include "unit_tests/uart.h"
#include "adc.h"
#include "SPI.h"
#include "oled.h"
#include "input.h"

//picocom command to read output from computer
//picocom -b 9600 -p 2 -r -l /dev/ttyS0
//git command for ask credentials on public computer:
//GIT_ASKPASS= git push https://github.com/leewberg/TTK4155_Byggern.git

int main(){
    uart_init(MYUBRR);
    SRAM_init();
    SPI_M_init();
	volatile const INPUT_DATA* data = input_init();
//	set_leds(&(LED_DATA){0b11111111});
    while (1){
	SPI_select_slave(AVR_s);
    SPI_M_write(0x05);
    _delay_us(40);
    SPI_M_write(0b11);
    _delay_us(2);
    SPI_M_write(0b1);

    _delay_ms(500);
	SPI_select_slave(AVR_s);
    SPI_M_write(0x05);
    _delay_us(40);
    SPI_M_write(0b11);
    _delay_us(2);
    SPI_M_write(0);
    }
	/*while(1){
		input_read( data);
		input_print(data);
		_delay_ms(1000);
	}*/
    return 0;
}
