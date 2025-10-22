#include "sam.h"
#include "const.h"

void pwm_init();
void pwm_set_duty_cycle(uint16_t duty_cycle);
void pwm_set_position(uint8_t pos);
uint8_t pwm_correct_duty_cycle();
