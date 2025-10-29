#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
#include "input.h"
#include "output.h"
#include "adc.h"
#include "encoder.h"
#include "time.h"
#include "motor_controller.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"
int BAUD = 9600;

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
	time_init();
    printf("Hello World\r\n");
    input_setup();
	output_init();
    adc_init();

    motor_init();
    CanMsg obj;
	CanMsg recv_msg;

    while (1){
        motor_control();
		// while (!can_rx(&recv_msg) || recv_msg.id != 2);
		// update_inputs(recv_msg.byte[0], recv_msg.byte[1], recv_msg.byte[2]);
		// pwm_set_duty_cycle(input_data->joy_x * 10);
    }
}
