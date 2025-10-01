#include "consts.h"
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"
#include "SRAM.h"
#include "unit_tests/latch.h"
#include "unit_tests/SRAM.h"
#include "unit_tests/uart.h"
// #include "adc.h"
#include "SPI.h"
#include "menu.h"
#include "oled.h"
#include "input.h"
#include "CAN.h"

//picocom command to read output from computer
//picocom -b 9600 -p 2 -r -l /dev/ttyS0
//git command for ask credentials on public computer:
//GIT_ASKPASS= git push https://github.com/leewberg/TTK4155_Byggern.git

int main(){
	uart_init(MYUBRR);
	SRAM_init();
    
    can_init();
    can_set_mode(MODE_LOOPBACK);
    printf("mode: %x\r\n", can_read(MCP_CANSTAT));


    can_write(MCP_TXB0SIDH, 0xA7);
    can_rts(0);
    uint8_t byte = can_read(MCP_TXB0SIDH);
    printf("mottar: %x\r\n", byte);
//	SPI_M_init();
	
    /*oled_init();
	volatile INPUT_DATA* data = input_init();
	Menu* menu = menu_init();
	int selected = -1;
	while(1){
		input_read(data);
		selected = menu_update(menu, data);
		if (selected != -1){
			printf(menu->items[selected].name);
			printf("\n\r");
			if (selected == 3){
				oled_reset();
				return 0;
			}
		}
	}*/
	return 0;
}
