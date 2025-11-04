#include <stdio.h>
#include <stdarg.h>
#include "display.h"
#include "doom_game/doom_runner.h"
#include "sam.h"
#include "time.h"
#include "uart.h"
#include "pwm.h"
#include "input.h"
#include "output.h"
#include "adc.h"
#include "encoder.h"
#include "motor_controller.h"

#define F_CPU 84000000

int main()
    /*TODO:
        decide CAN bus bit-timing by writing to CNFx on MCP2551 (match w/ CAN_BR register om ATSAM3X8E)
    */
{
    SystemInit();


    __enable_irq();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	

	PMC->PMC_PCER0 |= (1 << ID_PIOA); //send clock to PIOA
	PIOA->PIO_OER |= (1 << 6); //Set pin PA0 as output
	PIOA->PIO_SODR = (1 << 6); //Set pin PA0 high

    //Uncomment after including uart above
    uart_init(F_CPU, BAUD);
	
	// important to do this in this order
    input_setup();
	output_init();
	setupDisplay();
    motor_init();
	time_init();
	adc_init();


    printf("Hello World\r\n");

    while (1){
        motor_control();
		drawText(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT * .8, "TIME ", 1);
		// drawInt(100, 32, input_data->start_time);

		// check if timer overflowed.
		if (systick_ms - input_data->start_time >= play_time){
			printf("Time's up!\r\n");
			memset(display_buf, 0, SCREEN_WIDTH * SCREEN_HEIGHT / 8); // clear display
			run_doom();
		}
		printf("%d\r\n", systick_ms - input_data->start_time );
		print_trigger();
		can_transmit(); // push the frame
		
    }
}
