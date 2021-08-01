#define F_CPU 8000000  // 8MHz

#include <avr/io.h>
#include <util/delay.h>

#define INLINE static inline

INLINE void set_clock_to_8MHz (void);
INLINE void pin_PB0_as_PWM (void);
INLINE void pin_as_input_with_pull_up (uint8_t pin);
INLINE void pin_as_output (uint8_t pin);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

int main () {
  set_clock_to_8MHz();
  //pin_as_input_with_pull_up(PB0);
  pin_PB0_as_PWM();
  while (1) {
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

void pin_PB0_as_PWM (void) {
  /* Toggle OC0A (=PB0), fast PWM, OCR0A as TOP & use
     internal clock with prescaling 256.*/
  TCCR0A = _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(WGM03) | _BV(WGM02) | _BV(CS02);
  OCR0A = 50;  // f ~= 3000 Hz

  /* Toggle OC0A (=PB0) CTC mode & OCR0A as TOP & use internal clock with
     prescaling 64. */
  // TCCR0A = _BV(COM0A0);
  // TCCR0B = _BV(WGM02) | _BV(CS01) | _BV(CS00);
  // OCR0A = 15624;  // f = 0.5 Hz

  pin_as_output(PB0);
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
