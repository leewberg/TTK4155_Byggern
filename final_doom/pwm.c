#include "pwm.h"

void pwm_init() {
	PMC->PMC_PCER0 |= (1 << ID_PIOB); //send clock to PIOB
	PMC->PMC_PCER1 |= (1 << (ID_PWM - 32)); //send clock to PWM

	PIOB->PIO_PDR |= PIO_PB13; // disable PIO on PB13
	PIOB->PIO_ABSR |= PIO_PB13; // select peripheral B for PB13 (PWMH0)

	PIOB->PIO_PDR |= PIO_PB12; // disable PIO on PB12
	PIOB->PIO_ABSR |= PIO_PB12; // select peripheral B for PB12 (PWMH0)

	PWM->PWM_WPCR = 0x50574D << 8 | 0b111111 << 2; //Unlock write protection for all registers
	// read to check if write protection is disabled
	PWM->PWM_CLK = 0b0000 << 8 | 84 ; // sets PWM clock A to 1MHz (84MHz/84)
	REG_PWM_CMR1 = 0b1011;
	REG_PWM_CPRD1 = pwm_period; // period = 20ms, 50 Hz from 1MHz clock
	REG_PWM_CDTY1 = pwm_period - 2000; // 1.5 ms duty cycle

	REG_PWM_CMR0 = 0b1011;
	REG_PWM_CPRD0 = pwm_period; // period = 20ms, 50 Hz from 1MHz clock
	REG_PWM_CDTY0 = pwm_period - 2000; // 1.5 ms duty cycle
	
	PWM->PWM_ENA = 0b011; //enable channel 1
}
void pwm_set_duty_cycle(uint16_t duty_cycle) {
	if (duty_cycle > maxpwm) duty_cycle = maxpwm;
	if (duty_cycle < minpwm) duty_cycle = minpwm;

	REG_PWM_CDTYUPD1 = pwm_period - duty_cycle; // while enabled, write to update register
	
}

void motor_set_duty_cycle(uint16_t duty_cycle){
	if (duty_cycle < 4000) duty_cycle = 0;
	if (duty_cycle> pwm_period) duty_cycle = pwm_period;
	REG_PWM_CDTYUPD0 = pwm_period - duty_cycle;
}

void pwm_set_position(uint8_t pos){
	float newpos = (1 - pos/255.0)*1100+900;
	pwm_set_duty_cycle(newpos);
}