#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void pin_as_output (uint8_t pin);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

int main () {
  pin_as_output(PB4);
  while (1) {
    pin_on(PB4);
    _delay_ms(1000);
    pin_off(PB4);
    _delay_ms(1000);
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
