#include "motor_controller.h"
#include "time.h"

void motor_init(){

    pwm_init();
    encoder_init();

	PMC->PMC_PCER0 |= (1 << ID_PIOC); //send clock to PIOA
    PIOC->PIO_ABSR |= PIO_PC23;
    PIOC->PIO_PER |= PIO_PC23;
    PIOC->PIO_OER |= PIO_PC23;
    PIOC->PIO_CODR = PIO_PC23;

	// Solenoid controll: activate the pin
	PMC->PMC_PCER0 |= (1 << ID_PIOB);
	PMC->PMC_PCER0 |= (1 << ID_TC0); // correct PCER0?
	PIOA->PIO_PDR |= PIO_PB25; // disable PIO on PA4
	PIOA->PIO_ABSR &= ~PIO_PB25; // select peripheral A for PA4 (TIOA0)
	
	TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4; // waveform mode, clock/128, up mode with automatic
	TC0->TC_CHANNEL[0].TC_RA = 1;
	TC0->TC_CHANNEL[0].TC_RC = 6562;             // 10ms at 656250Hz
		
	TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN; // enable and start counter

	last_time = systick_ms;
	integral = 0.0;
}

void motor_control(){
	
	// solenoid, activate software trigger
	if (input_data->fire) {
		printf("trigger solenoid\r\n");
		TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;
	}

	// set the angle according to the slider
    pwm_set_position(input_data->slider);
	
	// sliding reference
	if (input_data->left) {
		ref += (double)(systick_ms - last_time) / 1000.0 * 0.5;
		if (ref > 1.0) {
			ref = 1.0;
		}
	}
	else if (input_data->right) {
		ref -= (double)(systick_ms - last_time) / 1000.0 * 0.5;
		if (ref < 0.0) {
			ref = 0.0;
		}
	}

	// read encoder value
    double encoder_val = encoder_read()/2805.0;
    if(encoder_val<0.0 || encoder_val > 1.0){
        encoder_val=0.0;
    }

    ref = 1-((double)input_data->slider)/255.0;
	// PI controller
    double error = ref - encoder_val;
	// integral += error * ((double)(systick_ms - last_time)/1000.0);

	double u = (error)*K_p + integral * K_i;

	// actuate the computed gain
    if(u < 0){
        PIOC->PIO_SODR = PIO_PC23;
        u = -u;
    }
    else{
        PIOC->PIO_CODR = PIO_PC23;
    }


    motor_set_duty_cycle(u);

	// bookeping

    printf("%f %4d %d \r\n", error, encoder_read(), input_data->slider);
	last_time = systick_ms;
}
