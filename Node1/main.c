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
#include "doom_handler.h"
#include <avr/wdt.h>

// picocom command to read output from computer
// picocom -b 9600 -p 2 -r -l /dev/ttyS0
// git command for ask credentials on public computer:
// GIT_ASKPASS= git push https://github.com/leewberg/TTK4155_Byggern.git

int main() {
	wdt_disable();
	uart_init(MYUBRR);
	// SRAM_init();

	doom_init();
	printf("Hello, World!\r\n");
	while (1) {
		doom_loop();
		printf("Frame done\r\n");
	}

	// can_init();
	// can_set_mode(MODE_NORMAL);
	// printf("mode: %x\r\n", can_read(MCP_CANSTAT));
	// can_frame msg;
	// msg.id = 0x00;
	// msg.length = 8;
	// msg.data[0] = 8;
	// msg.data[1] = 7;
	//
	// while(1){
	// 	while(!CAN_receive(&msg));
	// }
	
	// SPI_M_init();
	//
	// oled_init();
	// volatile INPUT_DATA* data = input_init();
	// Menu* menu = menu_init();
	// int selected = -1;
	// while(1){
	// 	input_read(data);
	// 	selected = menu_update(menu, data);
	// 	if (selected != -1){
	// 		printf(menu->items[selected].name);
	// 		printf("\n\r");
	// 		if (selected == 3){
	// 			oled_reset();
	// 			return 0;
	// 		}
	// 	}
	// }
	return 0;
}
