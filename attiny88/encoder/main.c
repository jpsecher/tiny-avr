#define F_CPU 8000000

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>

//
// Connections
//

// PA0 --- +Red LED- --- GND
#define A_CC_LED PA0

// PA1 --- +Green LED- --- GND
#define A_CV_LED PA1

//
// Buttons & Encoder use PCI2 (pins PDx)
//

// PD0 --- EncoderA --- GND
// PD1 --- EncoderA --- GND
#define D_V_ENC_A PD0
#define D_V_ENC_B PD1

#define SHORT_BLINK_ms 50
#define LONG_BLINK_ms 450

#define INLINE static inline
INLINE void ui_init (void);
INLINE void pin_as_output_A (uint8_t pin);
INLINE void pin_on_A (uint8_t pin);
INLINE void pin_off_A (uint8_t pin);
INLINE void pin_as_input_with_pull_up_D (uint8_t pin);
INLINE void setup_irq_on_buttons_and_encoders (void);
INLINE void pcint_23_to_16_generate_interupt_on_pci2 (void);
INLINE void enable_interupt_from_buttons_and_encoders (void);

bool last_A;
bool last_B;
bool a;
bool b;

int main () {
  ui_init();
  setup_irq_on_buttons_and_encoders();
  while (1) {
    if (a)
      pin_on_A(A_CC_LED);
    else
      pin_off_A(A_CC_LED);
    if (b)
      pin_on_A(A_CV_LED);
    else
      pin_off_A(A_CV_LED);
  }
  return 0;
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

//
// Encoder
//

ISR (PCINT2_vect) {
  a = bit_is_set(PIND, D_V_ENC_A);
  b = bit_is_set(PIND, D_V_ENC_B);
}

// void Encoder::sample ()
// {
//   uint8_t a = channelA.read();
//   uint8_t b = channelB.read();;
//   if (a != lastA || b != lastB)
//   {
//     translate(a, b);
//     lastA = a;
//     lastB = b;
//   }
// }

// void Encoder::translate (uint8_t a, uint8_t b)
// {
//   // 00 -> 01 -> 11 -> 10 -> ... = clockwise rotation.
//   if (
//     (lastA == 0 && lastB == 0 && a == 0 && b >= 1) ||
//     (lastA == 0 && lastB >= 1 && a >= 1 && b >= 1) ||
//     (lastA >= 1 && lastB >= 1 && a >= 1 && b == 0) ||
//     (lastA >= 1 && lastB == 0 && a == 0 && b == 0)
//   )
//   {
//     ++pulses;
//   }
//   // 01 -> 00 -> 10 -> 11 -> ... = counter clockwise rotation.
//   else if (
//     (lastA == 0 && lastB >= 1 && a == 0 && b == 0) ||
//     (lastA == 0 && lastB == 0 && a >= 1 && b == 0) ||
//     (lastA >= 1 && lastB == 0 && a >= 1 && b >= 1) ||
//     (lastA >= 1 && lastB >= 1 && a == 0 && b >= 1)
//   )
//   {
//     --pulses;
//   }
// }
