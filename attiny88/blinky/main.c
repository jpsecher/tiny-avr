/*
  Connections:

  PA0 --- +LED- --- GND
*/

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void pin_as_output (uint8_t pin);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

int main () {
  pin_as_output(PA0);
  while (1) {
    pin_on(PA0);
    _delay_ms(250);
    pin_off(PA0);
    _delay_ms(250);
  }
  return 0;
}

void pin_as_output (uint8_t pin) {
  DDRA |= _BV(pin);
}

void pin_on (uint8_t pin) {
  PORTA |= _BV(pin);
}

void pin_off (uint8_t pin) {
  PORTA &= ~_BV(pin);
}
