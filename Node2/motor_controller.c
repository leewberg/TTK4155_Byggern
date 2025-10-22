#include "motor_controller.h"

void motor_init(){

    pwm_init();
    encoder_init();

	PMC->PMC_PCER0 |= (1 << ID_PIOC); //send clock to PIOA
    PIOC->PIO_ABSR |= PIO_PC23;
    PIOC->PIO_PER |= PIO_PC23;
    PIOC->PIO_OER |= PIO_PC23;
    PIOC->PIO_CODR = PIO_PC23;
   	
}

void motor_control(){
    double encoder_val = encoder_read()/2805.0;
    if(encoder_val<0.0 || encoder_val > 1.0){
        encoder_val=0.0;
    }
    double ref = 1-((double)input_data->slider)/255.0;
    double error = ref - encoder_val;
    double K_p = 40000.0;
    
    if(error < 0){
        PIOC->PIO_SODR = PIO_PC23;
        error = -error;
    }
    else{
        PIOC->PIO_CODR = PIO_PC23;
    }
    printf("%f %4d %d \r\n", error, encoder_read(), input_data->slider);

    motor_set_duty_cycle((error)*K_p);
    pwm_set_position(input_data->slider);
}