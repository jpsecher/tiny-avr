/*
  Connections:

  PB0 --- +Buzzer- --- GND

  VCC --- 100kPOT --- GND
            |
           PB1
*/

#include <avr/io.h>

#define INLINE static inline
INLINE void set_clock_to_8MHz (void);
INLINE void pin_PB0_as_PWM (void);
INLINE void pin_PB1_as_ADC (void);
INLINE void start_ADC_convertion (void);
INLINE void busy_wait_for_ADC_convertion (void);
INLINE uint8_t scaled_ADC_value (void);
INLINE void pin_as_output (uint8_t pin);

int main () {
  set_clock_to_8MHz();
  pin_PB0_as_PWM();
  pin_PB1_as_ADC();
  while (1) {
    start_ADC_convertion();
    busy_wait_for_ADC_convertion();
    OCR0A = scaled_ADC_value();
  }
  return 0;
}

uint8_t scaled_ADC_value (void) {
  return (ADCL >> 2) + 0x03;
}

void set_clock_to_8MHz (void) {
  // Override configuration change protection
  CCP = 0xD8;
  // Set maximum speed (has to be done in the first 4 cycles after CCP)
  CLKPSR = 0;
}

void pin_PB0_as_PWM (void) {
  /* Toggle OC0A (=PB0), fast PWM, OCR0A as TOP & use
     internal clock with prescaling 256 (=> 32kHz) */
  TCCR0A = _BV(COM0A0) | _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(WGM03) | _BV(WGM02) | _BV(CS02);
  pin_as_output(PB0);
}

void pin_PB1_as_ADC (void) {
  // Use PB1 as input
  ADMUX = _BV(MUX0);
  // Enable ADC & prescale by 32 (=> 250kHz)
  ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);
  // Disable digital input on PB1
  DIDR0 |= _BV(ADC0D);
}

void start_ADC_convertion (void) {
  ADCSRA |= _BV(ADSC);
}

void busy_wait_for_ADC_convertion (void) {
  while (ADCSRA & _BV(ADSC));
}

void pin_as_output (uint8_t pin) {
  DDRB |= _BV(pin);
}
