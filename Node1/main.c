#include "SRAM.h"
#include "consts.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
// #include "adc.h"
#include "CAN.h"
#include "SPI.h"
#include "input.h"
#include "menu.h"
#include "oled.h"

// picocom command to read output from computer
// picocom -b 9600 -p 2 -r -l /dev/ttyS0
// git command for ask credentials on public computer:
// GIT_ASKPASS= git push https://github.com/leewberg/TTK4155_Byggern.git

int main() {
	uart_init(MYUBRR);
	SRAM_init();

	can_init();
	can_set_mode(MODE_NORMAL);
	printf("mode: %x\r\n", can_read(MCP_CANSTAT));

	while(1){
		printf("%4d", can_rs());
		can_rts(0);
		can_write(MCP_TXB0SIDH, 0b1);
	}
	
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
