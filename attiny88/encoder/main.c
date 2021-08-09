#define F_CPU 8000000

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

//
// Connections
//

// PA0 --- +Red LED- --- GND
#define A_CC_LED PA0

// PA1 --- +Green LED- --- GND
#define A_CV_LED PA1

// PD0 --- EncoderB --- GND
#define D_V_ENC_B PD0

// PD1 --- EncoderA --- GND
#define D_V_ENC_A PD1

#define INLINE static inline
INLINE void ui_init (void);
INLINE void pin_as_output_A (uint8_t pin);
INLINE void pin_on_A (uint8_t pin);
INLINE void pin_off_A (uint8_t pin);
INLINE void pin_as_input_with_pull_up_D (uint8_t pin);
INLINE void setup_irq_on_buttons_and_encoders (void);
INLINE void pcint_23_to_16_generate_interupt_on_pci2 (void);
INLINE void enable_interupt_from_buttons_and_encoders (void);
INLINE void translate_encoder (bool a, bool b);

bool a;
bool b;

ISR (PCINT2_vect) {
  a = bit_is_set(PIND, D_V_ENC_A);
  b = bit_is_set(PIND, D_V_ENC_B);
}

int main () {
  ui_init();
  setup_irq_on_buttons_and_encoders();
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
    pin_on_A(A_CV_LED);
  else
    pin_off_A(A_CV_LED);
  if (counter_clockwise)
    pin_on_A(A_CC_LED);
  else
    pin_off_A(A_CC_LED);
  _delay_ms(5);
}

void ui_init (void) {
  pin_as_output_A(A_CC_LED);
  pin_as_output_A(A_CV_LED);
  pin_as_input_with_pull_up_D(D_V_ENC_A);
  pin_as_input_with_pull_up_D(D_V_ENC_B);
}

void pin_as_output_A (uint8_t pin) {
  DDRA |= _BV(pin);
}

void pin_on_A (uint8_t pin) {
  PORTA |= _BV(pin);
}

void pin_off_A (uint8_t pin) {
  PORTA &= ~_BV(pin);
}

void pin_as_input_with_pull_up_D (uint8_t pin) {
  DDRD &= ~_BV(pin);
  PORTD |= _BV(pin);
}

void setup_irq_on_buttons_and_encoders (void) {
  pcint_23_to_16_generate_interupt_on_pci2();
  enable_interupt_from_buttons_and_encoders();
  sei();
}

void pcint_23_to_16_generate_interupt_on_pci2 (void) {
  PCICR |= _BV(PCIE2);
}

void enable_interupt_from_buttons_and_encoders (void) {
  PCMSK2 |= 0b11111111;
}
