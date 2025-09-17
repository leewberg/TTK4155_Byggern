#pragma once
#include "adc.h"
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>

void adc_test(void){
    volatile char *ext_ram = (char *) 0x1300; //start adress for SRAM
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    //rand() stores some internal state, so calling this function in a loop will yield different seeds each time (unless srand() is called this function)
    while(1){
        uint8_t retrieved_value = ext_ram[0];
    }

}

void adc_read(void){
    volatile char *adc_addr = (char *) 0x1300;
    volatile uint8_t out1 = 0;
    volatile uint8_t out2 = 0;
    volatile uint8_t out3 = 0;
    volatile uint8_t out4 = 0;
    while (1){
        // start adc conversion with write sig:
        adc_addr[0] = 0;
        _delay_us(CONVERSION_TIME*1000000);
        out1 = adc_addr[0];
        out2 = adc_addr[0];
        out3 = adc_addr[0];
        out4 = adc_addr[0];
        printf("Touchpad x: %4d Toucpad y: %4d Joystick x: %4d Joystick y: %4d  \n\r", out1, out4, out3, out2);
        
    } 
}

void CTC_init(void){
    DDRD |= (1<<PD5);
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |=(1<<WGM12);
    TCCR1A |= (1<<COM1A0);
    OCR1A = OCR1A_VAL;
    TCCR1B |= (1<<CS10);
    /*TCCR1A |= (1<<COM1A1) | (1<<WGM11) | (1<<WGM10);//0b10000011;
    TCCR1B |= (1<<WGM13) | (1<<WGM12);//0b00010001;
    ICR1 = 1;
    OCR1A = DUTY;*/
}