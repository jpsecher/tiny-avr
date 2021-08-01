#define F_CPU 1000000  // 1MHz because divided by 8 by default

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void pin_as_output (uint8_t pin);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

int main () {
  pin_as_output(PB0);
  pin_as_output(PB1);
  pin_as_output(PB2);
  pin_as_output(PB3);
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

void pin_as_output (uint8_t pin) {
  DDRB |= _BV(pin);
}

void pin_on (uint8_t pin) {
  PORTB |= _BV(pin);
}

void pin_off (uint8_t pin) {
  PORTB &= ~_BV(pin);
}
