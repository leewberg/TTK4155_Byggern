#pragma once
#include "latch.h"
#include <avr/io.h>
#include <avr/delay.h>

void latch_test() {
  DDRE = 0b10; // Set ALE as output
  PORTE = 0b10;
  DDRA = 0b11111111;
//  PORTA = 0b1;
//  PORTB = 0b1;
  DDRC = 0b1111;

  while(1){
    PORTE = 0b10; //Put ALE high. Tells the latch that it's recieving an adress it needs to save.
    PORTA = 0; // = 0x00. Sends an adress to the latch.

    _delay_ms(2000); // 2 sec delay for measuring.

    PORTE = 0b00; // Sets ALE low, and the adress is stored in the latch

    _delay_ms(4000);

    PORTA = 0b01010101;

    _delay_ms(2000);

    PORTE = 0b10;
    printf("loop complete! \n");
    _delay_ms(2000);

  }

}