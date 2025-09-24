#include "SPI.h"

/*Pins:
MOSI: PB5 (Master: DDB6)
MISO PB6 (Master: DDB5)
SCK: PB7 (Master: DDB7)
!(SS): PB4 (Master: DDB4)

PB1 = SS for CAN
PB2 = DISP_D/C for data/command select for the oled
PB3 = DISP_CS chip select for the oled
PB4 = IO_CS is AVR chip select
*/

void SPI_M_init(void){
    //Sets MISO as input, all others as output
    DDRB = (1<<DDB7) | (0 << DDB6) | (1 << DDB5) | (1 << DDB4) |  (1 << DDB3) | (1 << DDB2) | (1 << DDB1);
    //Enable SPI, Master, set clock rate as fck/16
    SPCR = (1<<SPE) | (1 << MSTR) | (1 <<SPR0);
    //Use SPI-Mode 0 (table on p 165 in datasheet). since the rest of SPCR is 0 (except for SPE, MSTR, and SPR0), we don't need to explicitly set CPOL and CPHA, but if you choose another mode, you knwo which signals to change :]
    //see p. 163 in datasheet if you want to change SPI clock rate.
    //if you want to enable interrupts: | (1<<SPIE);
    //Set SS-pin high
}

void SPI_M_write(char cData){
    //Start transmission
    SPDR = cData;
    //Wait for transmission to complete
    while(!(SPSR & (1<<SPIF)));
}

char SPI_M_read(void){
    //Wait for reception to complete
    while(!(SPSR & (1<<SPIF)));
    //Return data register
    return SPDR;
}

void SPI_select_slave(enum SLAVES slave){    
    if (slave == CAN){
        PORTB = (1 << DDB4) | (1 << DDB3) | (1 << DDB2) | (0 << DDB1);
    }
    else if (slave == AVR){
        PORTB = (0 << DDB4) | (1 << DDB3) | (1 << DDB2) | (1 << DDB1);
    }
    else if (slave == OLED_CMD){
        PORTB = (1 << DDB4) | (0 << DDB3) | (0 << DDB2) | (1 << DDB1);
    }
    else if (slave == OLED_DATA){
        PORTB = (1 << DDB4) | (0 << DDB3) | (1 << DDB2) | (1 << DDB1);
    }
}