/*
  Connections:

  PB0 --- PushButton --- GND

  PB1 --- +LED- --- GND
*/

#define F_CPU 8000000  // 8MHz

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void set_clock_to_8MHz (void);
INLINE void pin_as_input_with_pull_up (uint8_t pin);
INLINE void pin_as_output (uint8_t pin);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

int main () {
  set_clock_to_8MHz();
  pin_as_input_with_pull_up(PB0);
  pin_as_output(PB1);
  while (1) {
    if (PINB & _BV(PB0)) {
      pin_off(PB1);
    } else {
      pin_on(PB1);
    }
    _delay_ms(1);
  }
  return 0;
}

void set_clock_to_8MHz (void) {
  // Override configuration change protection
  CCP = 0xD8;
  // Set maximum speed (has to be done in the first 4 cycles after CCP)
  CLKPSR = 0;
}

void pin_as_input_with_pull_up (uint8_t pin) {
  DDRB &= ~_BV(pin);
  PUEB |= _BV(pin);
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
