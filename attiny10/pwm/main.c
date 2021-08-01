#define F_CPU 8000000  // 8MHz

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void set_clock_to_8MHz (void);
INLINE void all_pins_as_output (void);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

int main () {
  set_clock_to_8MHz();
  all_pins_as_output();
  while (1) {
    pin_on(PB3);
    _delay_ms(100);
    pin_off(PB3);
    pin_on(PB2);
    _delay_ms(100);
    pin_off(PB2);
    pin_on(PB1);
    _delay_ms(100);
    pin_off(PB1);
    pin_on(PB0);
    _delay_ms(100);
    pin_off(PB0);
    _delay_ms(400);
  }
  return 0;
}

void set_clock_to_8MHz (void) {
  // Override configuration change protection
  CCP = 0xD8;
  // Set maximum speed (has to be done in the first 4 cycles after CCP)
  CLKPSR = 0;
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
