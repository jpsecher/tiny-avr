/*
  Connections:

  PB0 --- +LED- --- GND

  PB1 --- +LED- --- GND

  PB2 --- +LED- --- GND

  PB3 --- +LED- --- GND
*/

#define F_CPU 1000000  // 1MHz because divided by 8 by default

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void all_pins_as_output (void);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

int main () {
  all_pins_as_output();
  while (1) {
    pin_on(PB0);
    _delay_ms(100);
    pin_off(PB0);
    pin_on(PB1);
    _delay_ms(100);
    pin_off(PB1);
    pin_on(PB2);
    _delay_ms(100);
    pin_off(PB2);
    pin_on(PB3);
    _delay_ms(100);
    pin_off(PB3);
    _delay_ms(500);
  }
  return 0;
}

void all_pins_as_output (void) {
  DDRB = _BV(PB0) | _BV(PB1) | _BV(PB2) | _BV(PB3);
}

void pin_on (uint8_t pin) {
  PORTB |= _BV(pin);
}

void pin_off (uint8_t pin) {
  PORTB &= ~_BV(pin);
}
