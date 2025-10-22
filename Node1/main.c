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

	doom_init();
	printf("Hello, World!\r\n");
	while (1) {
		fetch_and_send_inputs();
		_delay_us(1000);
	}


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
