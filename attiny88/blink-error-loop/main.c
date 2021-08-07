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

// uC --- SW_PUSH --- GND
#define D_OE_SW PD2

#define SHORT_BLINK_ms 50
#define LONG_BLINK_ms 450


enum Status {
  // Non-errors.
  S_ok,
  S_processing_needed,
  // Errors.
  S_memory_layout_error,
  S_unknown_error,
  S_end_marker
};

#define N_INTR_HANDLERS 3
uint8_t intr_handler_data_layout_H [N_INTR_HANDLERS];
uint8_t intr_handler_to_data_index_H [N_INTR_HANDLERS];

#define N_INTR_TOTAL_BYTES 5
uint8_t intr_handler_data [N_INTR_TOTAL_BYTES];

enum Intr_Handler {
  H_sanity,
  H_my_handler,
  H_other_handler
};

#define INLINE static inline

INLINE void ui_init (void);
INLINE void init_intr_handler_tables (void);
INLINE void set_status_H_S (uint8_t handler, uint8_t status);
INLINE void set_data_H_n_b (uint8_t handler, uint8_t index, uint8_t data);
INLINE uint8_t get_status_H (uint8_t handler);
INLINE uint8_t get_data_H_n (uint8_t handler, uint8_t index);
INLINE void init_intr_handler_data (void);
INLINE void sanity_check (void);
INLINE void simulate_error (void);
INLINE bool error_present (void);
INLINE void show_all_errors (void);
INLINE void show_error (uint8_t status);
INLINE void turn_off_output (void);
INLINE void pin_as_output_A (uint8_t pin);
INLINE void pin_on_A (uint8_t pin);
INLINE void pin_off_A (uint8_t pin);
INLINE void pin_as_input_with_pull_up_D (uint8_t pin);
INLINE void setup_irq_on_buttons_and_encoders (void);
INLINE void pcint_23_to_16_generate_interupt_on_pci2 (void);
INLINE void enable_interupt_from_buttons_and_encoders (void);


int main () {
  init_intr_handler_tables();
  ui_init();
  setup_irq_on_buttons_and_encoders();
  sanity_check();
  // simulate_error();  // <-------
  while (1) {
    while (error_present()) {
      turn_off_output();
      show_all_errors();
      _delay_ms(LONG_BLINK_ms);
    }
    // Normal processing...
    if (bit_is_clear(PIND, D_OE_SW))
      pin_on_A(A_CV_LED);
    else
      pin_off_A(A_CV_LED);
  }
  return 0;
}

void simulate_error (void) {
  set_status_H_S(H_sanity, S_memory_layout_error);
  set_status_H_S(H_other_handler, S_unknown_error);
}

bool error_present (void) {
  uint8_t handler = 0;
  uint8_t status;
  for (; handler < N_INTR_HANDLERS; ++handler) {
    status = get_status_H(handler);
    if (status >= S_memory_layout_error)
      return true;
  }
  return false;
}

void show_all_errors (void) {
  uint8_t handler = 0;
  uint8_t status;
  for (; handler < N_INTR_HANDLERS; ++handler) {
    status = get_status_H(handler);
    if (status >= S_memory_layout_error) {
      show_error(status);
    }
  }
}

void show_error (uint8_t status) {
  pin_off_A(A_CV_LED);
  // Blink red to signal error.
  pin_on_A(A_CC_LED);
  _delay_ms(LONG_BLINK_ms);
  pin_off_A(A_CC_LED);
  _delay_ms(LONG_BLINK_ms);
  while (status != 0) {
    // Blink green, short for 0, long for 1.
    pin_on_A(A_CV_LED);
    if ((status & 1) == 1) {
      _delay_ms(LONG_BLINK_ms);
      pin_off_A(A_CV_LED);
    }
    else {
      _delay_ms(SHORT_BLINK_ms);
      pin_off_A(A_CV_LED);
    }
    _delay_ms(LONG_BLINK_ms);
    status >>= 1;
  }
}

void sanity_check (void) {
  // Data area must be at least as big as the total usage by the Interrupt
  // handlers.
  uint8_t handler = 0;
  uint8_t total_bytes = 0;
  for (; handler < N_INTR_HANDLERS; ++handler)
    total_bytes += intr_handler_data_layout_H[handler];
  if (N_INTR_TOTAL_BYTES < total_bytes) {
    intr_handler_data[H_sanity] = S_memory_layout_error;
  }
}

void init_intr_handler_tables (void) {
  intr_handler_data_layout_H[H_sanity] = 1;  // status byte for sanity check
  intr_handler_data_layout_H[H_my_handler] = 3;  // my_intr_handler uses status byte and two value bytes
  intr_handler_data_layout_H[H_other_handler] = 1;  // another_intr_handler only uses status byte
  init_intr_handler_data();
};

void init_intr_handler_data (void) {
  uint8_t handler = 0;
  uint8_t data_index = 0;
  for (; handler < N_INTR_HANDLERS; ++handler) {
    intr_handler_to_data_index_H[handler] = data_index;
    data_index += intr_handler_data_layout_H[handler];
  }
  memset(intr_handler_data, S_ok, N_INTR_TOTAL_BYTES);
}

void turn_off_output (void) {
  // TODO
}

void set_status_H_S (uint8_t handler, uint8_t status) {
  set_data_H_n_b(handler, 0,  status);
}

void set_data_H_n_b (uint8_t handler, uint8_t index, uint8_t data) {
  intr_handler_data[intr_handler_to_data_index_H[handler]+index] = data;
}

uint8_t get_status_H (uint8_t handler) {
  return get_data_H_n(handler, 0);
}

uint8_t get_data_H_n (uint8_t handler, uint8_t index) {
  return intr_handler_data[intr_handler_to_data_index_H[handler]+index];
}

void ui_init (void) {
  pin_as_output_A(A_CC_LED);
  pin_as_output_A(A_CV_LED);
  pin_as_input_with_pull_up_D(D_OE_SW);
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

ISR (PCINT2_vect) {
  // IO pulled down means button pressed.
  if (bit_is_clear(PIND, D_OE_SW))
    set_status_H_S(H_other_handler, S_end_marker);
}
