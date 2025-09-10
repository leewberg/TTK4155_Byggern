#include "SRAM.h"

void SRAM_init(){
    MCUCR = (1 <<SRE);
    SFIOR = (1<<XMM2);
}


void SRAM_write(uint8_t data, uint16_t adr){
    volatile char *ext_mem = (char*) BASE_ADRESS;
    ext_mem[adr] = data;
}

uint8_t SRAM_read(uint16_t adr){
    volatile char *ext_mem = (char *) BASE_ADRESS;
    uint8_t ret_val = ext_mem[adr];
    return ret_val;
}