#include "adc.h"
#include <stdio.h>

void PWM_init(void){
    DDRD |= (1<<PD5);
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |=(1<<WGM12);
    TCCR1A |= (1<<COM1A0);
    OCR1A = 0b11;
    TCCR1B |= (1<<CS10);
    /*TCCR1A |= (1<<COM1A1) | (1<<WGM11) | (1<<WGM10);//0b10000011;
    TCCR1B |= (1<<WGM13) | (1<<WGM12);//0b00010001;
    ICR1 = 1;
    OCR1A = DUTY;*/
}