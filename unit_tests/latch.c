#pragma once
#include "latch.h"
#include <avr/io.h>
#include <avr/delay.h>
// mulig avr/delay.h ikke funker, og du må bruke følgende:
// #define F_CPU 4915200
// #include <util/delay.h>

void latch_test() {
  DDRE = 0b10; // Setter ALE (pin 1 på port E) som utgang. 0=inngang, 1=utgang
  PORTE = 0b10;
  DDRA = 0b11111111;
//  PORTA = 0b1;
//  PORTB = 0b1;
  DDRC = 0b1111;

  while(1){
    PORTE = 0b10; // Setter ALE høy. Forteller vippa at nå kommer det en adresse som skal lagres.
    PORTA = 0; // = 0x00. Sender en adresse ut til vippa.

    _delay_ms(2000); // 2 sek delay for å gjøre det enklere å måle på kretsen.

    PORTE = 0b00; // Setter ALE lav. Nå lagres adresseverdien i vippa.

    _delay_ms(4000);

    PORTA = 0b01010101; // Sender ut ny adresse. Ingenting skjer på baksiden av vippa.

    _delay_ms(2000);

    PORTE = 0b10; // Gammel adresse fjernes fra vippa og den nye sendes gjennom.
    printf("loop complete! \n");
    _delay_ms(2000);

  }

}