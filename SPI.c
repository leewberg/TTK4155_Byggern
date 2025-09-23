#include "SPI.h"

/*Pins:
MOSI: PB5 (Master: DDB6)
MISO PB6 (Master: DDB5)
SCK: PB7 (Master: DDB7)
!(SS): PB4 (Master: DDB4)
*/

void SPI_M_init(void){
    //Sets MOSI and SCK as output, all others as input
    DDRB = (1<<DDB6) | (1 << DDB7);
    //Enable SPI, Master, set clock rate as fck/16
    SPCR = (1<<SPE) | (1 << MSTR) | (1 <<SPR0);
    //Use SPI-Mode 0 (table on p 165 in datasheet). since the rest of SPCR is 0 (except for SPE, MSTR, and SPR0), we don't need to explicitly set CPOL and CPHA, but if you choose another mode, you knwo which signals to change :]
    //see p. 163 in datasheet if you want to change SPI clock rate.
    //if you want to enable interrupts: | (1<<SPIE);
    //Set SS-pin high
    DDRB |= (1<<DDB4);
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