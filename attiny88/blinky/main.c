/*
  Connections:

  PD6 --- +LED(R)- --- GND
  PD7 --- +LED(G)- --- GND
*/

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void pin_D_as_output (uint8_t pin);
INLINE void pin_D_on (uint8_t pin);
INLINE void pin_D_off (uint8_t pin);

int main () {
  pin_D_as_output(PD6);
  pin_D_as_output(PD7);
  while (1) {
    pin_D_on(PD6);
    pin_D_off(PD7);
    _delay_ms(250);
    pin_D_off(PD6);
    pin_D_on(PD7);
    _delay_ms(500);
  }
  return 0;
}

void pin_D_as_output (uint8_t pin) {
  DDRD |= _BV(pin);
}

void pin_D_on (uint8_t pin) {
  PORTD |= _BV(pin);
}

void pin_D_off (uint8_t pin) {
  PORTD &= ~_BV(pin);
}
