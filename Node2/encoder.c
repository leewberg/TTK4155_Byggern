#include "encoder.h"

void encoder_init(void){
    /*implement encoder signal reader on QUAD pins
    PINS PC26 and PC25
    Implement PWM generator on ENABLE/PWM and PHASE/DIR
    create discrete P controller
    
    use TC2 CH0
    encoder mode
    */
    PMC->PMC_PCER0 |= (1 << ID_PIOC); //send clock to PIOC
    PMC->PMC_PCER1 |= (1 << (ID_TC6 - 32)); //send clock to PWM
	PIOB->PIO_PDR |= PIO_PC25; // disable PIO on PC25
	PIOB->PIO_ABSR |= PIO_PC25; // select peripheral B for PC25 (PWMH0)
	PIOB->PIO_PDR |= PIO_PC26; // disable PIO on PB12
	PIOB->PIO_ABSR |= PIO_PC26; // select peripheral B for PB12 (PWMH0)
	


    REG_TC2_WPMR = 0b1 | 0x54494D;
    REG_TC2_CCR0 = 0b101;
    REG_TC2_CMR0 = 5; //Clock selected: 0C0
    REG_TC2_BMR = (1<<TC_BMR_QDEN) | (1<<TC_BMR_POSEN); //enables position measure on channel 0 and 1. enables QDEC
}

uint32_t encoder_read(){
    return REG_TC2_CV0;
}