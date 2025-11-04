#include "adc.h"

volatile uint8_t adc_triggered = 0;

void adc_init(void){
    PMC -> PMC_WPMR = PMC_WPMR_WPKEY(0x504d43)& ~ PMC_WPMR_WPEN;
    PMC -> PMC_PCER1 |= (1<<(ID_ADC-32));
    PMC -> PMC_WPMR = PMC_WPMR_WPKEY(0x504d43)& PMC_WPMR_WPEN;

    ADC -> ADC_WPMR = ADC_WPMR_WPKEY(0x414443) & ~ADC_WPMR_WPEN;
    ADC -> ADC_CHER = ADC_CHER_CH0;
    ADC -> ADC_EMR = ADC_EMR_CMPMODE_LOW;
    ADC -> ADC_CWR = ADC_CWR_LOWTHRES(LOWTRESHOLD);
    ADC -> ADC_IER = ADC_IER_COMPE;
    NVIC_EnableIRQ(ADC_IRQn);
    ADC-> ADC_MR |= ADC_MR_FREERUN_ON;
    ADC->ADC_CR = ADC_CR_START;
    ADC -> ADC_WPMR = ADC_WPMR_WPEN;
}


void ADC_Handler(void){
    uint32_t status = ADC-> ADC_ISR;
    if (status&ADC_ISR_COMPE){
        adc_triggered=1;
    }
}

uint16_t ADC_polling(void){

	ADC->ADC_CR = ADC_CR_START; // Start conversion
	while (!(ADC->ADC_ISR & ADC_ISR_EOC0)); // Wait for end of conversion
	return ADC->ADC_CDR[0]; // Read result
}

void print_trigger(){
    if (adc_triggered){
        adc_triggered = 0;
        printf("ADC interrupt\r\n");
    }
}
