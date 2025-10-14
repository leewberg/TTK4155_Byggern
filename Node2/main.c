#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "uart.h"
#include "can.h"

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
#define F_CPU 84000000

int main()
    /*TODO:
        decide CAN bus bit-timing by writing to CNFx on MCP2551 (match w/ CAN_BR register om ATSAM3X8E)
    */
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
	PMC->PMC_PCER0 |= (1 << ID_PIOA); //send clock to PIOA
	PIOA->PIO_OER |= (1 << 6); //Set pin PA0 as output
	PIOA->PIO_SODR = (1 << 6); //Set pin PA0 high

    //Uncomment after including uart above
    uart_init(F_CPU, BAUD);

    CanInit canInit;
    canInit.brp = 104;
    canInit.phase1 = 2;
    canInit.phase2 = 2;
    canInit.propag = 0;
    canInit.smp = 0;
    canInit.sjw = 0;
    can_init(canInit, 0);
    printf("Hello World\r\n");

    CanMsg obj;

    while (1)
    {   
//        can_rx(&obj);
		obj.id = 0x12;	
		obj.length = 2;
		obj.byte[0] = 0xFF;
		obj.byte[1] = 0x00;
		can_tx(obj);
		int i = 0;
		while(i < 1000000) {i++;}
        /* code */
    }
}
