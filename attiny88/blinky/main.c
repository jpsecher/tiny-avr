/*
  Connections:

  PA0 --- +LED- --- GND
  PA1 --- +LED- --- GND
*/

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void pin_A_as_output (uint8_t pin);
INLINE void pin_A_on (uint8_t pin);
INLINE void pin_A_off (uint8_t pin);

int main () {
  pin_A_as_output(PA0);
  pin_A_as_output(PA1);
  while (1) {
    pin_A_on(PA0);
    pin_A_off(PA1);
    _delay_ms(250);
    pin_A_off(PA0);
    pin_A_on(PA1);
    _delay_ms(250);
  }
  return 0;
}

void pin_A_as_output (uint8_t pin) {
  DDRA |= _BV(pin);
}

void pin_A_on (uint8_t pin) {
  PORTA |= _BV(pin);
}

void pin_A_off (uint8_t pin) {
  PORTA &= ~_BV(pin);
}
