#define F_CPU 8000000

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

//
// Connections
//

//  PD6 --- +LED(R)- --- GND
#define D_CC_LED PD6

//  PD7 --- +LED(G)- --- GND
#define D_CV_LED PD7


// PB0 --- EncoderB --- GND
#define B_V_ENC_B PB0

// PB1 --- EncoderA --- GND
#define B_V_ENC_A PB1

#define INLINE static inline
INLINE void ui_init (void);
INLINE void pin_as_output_D (uint8_t pin);
INLINE void pin_on_D (uint8_t pin);
INLINE void pin_off_D (uint8_t pin);
INLINE void pin_as_input_with_pull_up_B (uint8_t pin);
INLINE void setup_irq_on_encoder (void);
INLINE void pcint_7_to_0_generate_interrupt_on_pci0 (void);
INLINE void enable_interrupt_from_encoder (void);
INLINE void translate_encoder (bool a, bool b);

bool a;
bool b;

ISR (PCINT0_vect) {
  a = bit_is_set(PINB, B_V_ENC_A);
  b = bit_is_set(PINB, B_V_ENC_B);
}

int main () {
  ui_init();
  setup_irq_on_encoder();
  while (1) {
    translate_encoder(a, b);
  }
  return 0;
}

void translate_encoder (bool a, bool b) {
  static bool last_A = false;
  static bool last_B = false;
  bool clockwise = false;
  bool counter_clockwise = false;
  if (a != last_A || b != last_B)
  {
    // 00 -> 01 -> 11 -> 10 -> ... = clockwise rotation.
    if (
      (!last_A && !last_B && !a && b) ||
      (!last_A && last_B && a && b) ||
      (last_A && last_B && a && !b) ||
      (last_A && !last_B && !a && !b)
    )
    {
      clockwise = true;
    }
    // 01 -> 00 -> 10 -> 11 -> ... = counter clockwise rotation.
    else if (
      (!last_A && last_B && !a && !b) ||
      (!last_A && !last_B && a && !b) ||
      (last_A && !last_B && a && b) ||
      (last_A && last_B && !a && b)
    )
    {
      counter_clockwise = true;
    }
    last_A = a;
    last_B = b;
  }
  if (clockwise)
    pin_on_D(D_CV_LED);
  else
    pin_off_D(D_CV_LED);
  if (counter_clockwise)
    pin_on_D(D_CC_LED);
  else
    pin_off_D(D_CC_LED);
  _delay_ms(5);
}

void ui_init (void) {
  pin_as_output_D(D_CC_LED);
  pin_as_output_D(D_CV_LED);
  pin_as_input_with_pull_up_B(B_V_ENC_A);
  pin_as_input_with_pull_up_B(B_V_ENC_B);
}

void pin_as_output_D (uint8_t pin) {
  DDRD |= _BV(pin);
}

void pin_on_D (uint8_t pin) {
  PORTD |= _BV(pin);
}

void pin_off_D (uint8_t pin) {
  PORTD &= ~_BV(pin);
}

void pin_as_input_with_pull_up_B (uint8_t pin) {
  DDRB &= ~_BV(pin);
  PORTB |= _BV(pin);
}

void setup_irq_on_encoder (void) {
  pcint_7_to_0_generate_interrupt_on_pci0();
  enable_interrupt_from_encoder();
  sei();
}

void pcint_7_to_0_generate_interrupt_on_pci0 (void) {
  PCICR |= _BV(PCIE0);
}

void enable_interrupt_from_encoder (void) {
  PCMSK0 |= 0b11111111;
}
