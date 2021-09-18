//
// Connections
// ===========
//
// All buttons & encoder use PCI0 (pins PBx).
// LEDs on PDx.
// ADC on PC0/1.

// When A_I_SENSE is over half of the input range (0x1FF), then the red LED is on.
// When A_V_SENSE is over half of the input range (0x1FF), then the gree LED is on.
// When button is pushed, TODO: the ADC reference changes to 1.1V

//  PD6 --- +LED(R)- --- GND
#define D_CC_LED PD6

//  PD7 --- +LED(G)- --- GND
#define D_CV_LED PD7

// PB2 --- SW_PUSH --- GND
#define B_OE_SW PB2

// PA0 --- I-sense
#define A_I_SENSE PA0

// PA1 --- V_sense
#define A_V_SENSE PA1


#define F_CPU 8000000
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>

//
// Timings
//
#define SHORT_BLINK_ms 50
#define LONG_BLINK_ms 450

enum Status {
  // Non-errors.
  S_ok = 0,
  S_processing_needed = 1,
  // System errors.
  S_memory_layout_error = 2,
  S_unknown_error = 3,
  // ADC errors.
  S_adc_unknown = 4,
  S_adc_test = 5,
};

enum Intr_Handler {
  H_sanity,
  H_button,
  H_adc,
};

// Has to match the number of interrupt handlers listed above.
#define N_INTR_HANDLERS 3
uint8_t intr_handler_data_layout_H [N_INTR_HANDLERS];
uint8_t intr_handler_to_data_index_H [N_INTR_HANDLERS];

// Has to match (or exceed) number of bytes reserved for all interrupt handlers.
#define N_INTR_TOTAL_BYTES 14
uint8_t intr_handler_data [N_INTR_TOTAL_BYTES];

typedef void (*handler_t)(void);
handler_t intr_handler_H [N_INTR_HANDLERS];

// Common return values.
#define SUCCESS 0
#define FAIL 1
#define BUSY 2
#define return_t uint8_t

#define INLINE static inline
INLINE void ui_init (void);
INLINE void init_intr_handler_tables (void);
INLINE void set_status_H_S (uint8_t handler, uint8_t status);
INLINE void set_data_H_n_b (uint8_t handler, uint8_t index, uint8_t data);
INLINE void set_data_H_n_w (uint8_t handler, uint8_t index, uint16_t data);
INLINE uint8_t get_status_H (uint8_t handler);
INLINE uint8_t get_b_data_H_n (uint8_t handler, uint8_t index);
INLINE uint16_t get_w_data_H_n (uint8_t handler, uint8_t index);
INLINE void init_intr_handler_data (void);
INLINE void sanity_check (void);
INLINE bool error_present (void);
INLINE void show_all_errors (void);
INLINE void show_error (uint8_t status);
INLINE void process_all_events (void);
INLINE void pin_as_output_D (uint8_t pin);
INLINE void pin_on_D (uint8_t pin);
INLINE void pin_off_D (uint8_t pin);
INLINE void pin_as_input_with_pull_up_B (uint8_t pin);
INLINE void setup_irq_on_button (void);
INLINE void enable_interrupt_from_buttons (void);
INLINE void pcint_7_to_0_generate_interrupt_on_pci0 (void);
INLINE void init_sanity_handler (uint8_t handler);
INLINE void init_button_handler (uint8_t handler);
INLINE void init_adc_handler (uint8_t handler);
INLINE void adc_setup (void);
INLINE void adc_free_running_interrupt_at_125kHz (void);
INLINE void adc_current_high (void);
INLINE void adc_current_low (void);INLINE void adc_voltage_high (void);
INLINE void adc_voltage_low (void);
INLINE void adc_effectuate_channel_and_ref (void);
INLINE void adc_use_channels_6_7 (void);


// Functions tied to specific Intr_Handlers.
void unknown_error (void);
void h_button (void);
void h_adc (void);

int main () {
  init_intr_handler_tables();
  ui_init();
  setup_irq_on_button();
  adc_setup();
  sanity_check();
  sei();
  while (1) {    while (error_present()) {
      show_all_errors();
      _delay_ms(LONG_BLINK_ms);
    }
    process_all_events();
  }
  return 0;
}

void process_all_events (void) {
  uint8_t handler = 0;
  uint8_t status;
  for (; handler < N_INTR_HANDLERS; ++handler) {
    status = get_status_H(handler);
    if (status == S_processing_needed)
      (*intr_handler_H[handler])();
  }
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
  pin_off_D(D_CV_LED);
  // Blink red to signal error.
  pin_on_D(D_CC_LED);
  _delay_ms(LONG_BLINK_ms);
  pin_off_D(D_CC_LED);
  _delay_ms(LONG_BLINK_ms);
  while (status != 0) {
    // Blink green, short for 0, long for 1.
    pin_on_D(D_CV_LED);
    if ((status & 1) == 1) {
      _delay_ms(LONG_BLINK_ms);
      pin_off_D(D_CV_LED);
    }
    else {
      _delay_ms(SHORT_BLINK_ms);
      pin_off_D(D_CV_LED);
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
  if (N_INTR_TOTAL_BYTES < total_bytes)
    set_status_H_S(H_sanity, S_memory_layout_error);
}

void init_intr_handler_tables (void) {
  init_sanity_handler(H_sanity);
  init_button_handler(H_button);
  init_adc_handler(H_adc);
  init_intr_handler_data();
};

void init_sanity_handler (uint8_t handler) {
  // Only status byte.
  intr_handler_data_layout_H[handler] = 1;
  intr_handler_H[handler] = unknown_error;
}

void unknown_error (void) {
  set_status_H_S(H_sanity, S_unknown_error);
}

void init_button_handler (uint8_t handler) {
  // Status byte and a bytes for the button (1 bit).
  intr_handler_data_layout_H[handler] = 2;
  intr_handler_H[handler] = h_button;
}

void init_adc_handler (uint8_t handler) {
  // Status byte, channel, conversion to skip, array of readings (2 * (V + I))
  intr_handler_data_layout_H[handler] = 3 + 2 * 2;
  intr_handler_H[handler] = h_adc;
}

void init_intr_handler_data (void) {
  uint8_t handler = 0;
  uint8_t data_index = 0;
  for (; handler < N_INTR_HANDLERS; ++handler) {
    intr_handler_to_data_index_H[handler] = data_index;
    data_index += intr_handler_data_layout_H[handler];
  }
  memset(intr_handler_data, S_ok, N_INTR_TOTAL_BYTES);
}

void set_status_H_S (uint8_t handler, uint8_t status) {
  set_data_H_n_b(handler, 0,  status);
}

void set_data_H_n_b (uint8_t handler, uint8_t index, uint8_t data) {
  intr_handler_data[intr_handler_to_data_index_H[handler]+index] = data;
}

void set_data_H_n_w (uint8_t handler, uint8_t index, uint16_t data) {
  intr_handler_data[intr_handler_to_data_index_H[handler]+index] = data & 0xFF;
  intr_handler_data[intr_handler_to_data_index_H[handler]+index+1] = data >> 8;
}

uint8_t get_status_H (uint8_t handler) {
  return get_b_data_H_n(handler, 0);
}

uint8_t get_b_data_H_n (uint8_t handler, uint8_t index) {
  return intr_handler_data[intr_handler_to_data_index_H[handler]+index];
}

uint16_t get_w_data_H_n (uint8_t handler, uint8_t index) {
  return intr_handler_data[intr_handler_to_data_index_H[handler]+index] |
    intr_handler_data[intr_handler_to_data_index_H[handler]+index+1] << 8;
}

void ui_init (void) {
  pin_as_output_D(D_CC_LED);
  pin_as_output_D(D_CV_LED);
  pin_as_input_with_pull_up_B(B_OE_SW);
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

void setup_irq_on_button (void) {
  pcint_7_to_0_generate_interrupt_on_pci0();
  enable_interrupt_from_buttons();
}

void pcint_7_to_0_generate_interrupt_on_pci0 (void) {
  PCICR |= _BV(PCIE0);
}

void enable_interrupt_from_buttons (void) {
  // B_OE_SW == PCINT2 will generate interrupt.
  PCMSK0 |= _BV(PCINT2);
}

//
// Button
//

ISR (PCINT0_vect) {
  // IO pulled down means button pressed.
  if (bit_is_clear(PINB, B_OE_SW)) {
    set_data_H_n_b(H_button, 1, 0xFF);
  }
  set_status_H_S(H_button, S_processing_needed);
}

void h_button (void) {
  uint8_t status;
  uint8_t state;
  cli();
  status = get_status_H(H_button);
  state = get_b_data_H_n(H_button, 1);
  set_status_H_S(H_button, S_ok);
  set_data_H_n_b(H_button, 1, 0x00);
  sei();
  if (status == S_processing_needed) {
    if (state) {
      return set_status_H_S(H_adc, S_adc_test);
    }
  }
}


//
// ADC
//

// PA0 = ADC6 = 0110
// PA1 = ADC7 = 0111
// TEM = ADC8 = 1000
#define ADC_CURRENT_PORT 6
#define ADC_VOLTAGE_PORT 7

// Storage layout
#define ADC_CHANNEL_AND_REF 1
#define ADC_READINGS_TO_SKIP 2
#define ADC_VOLTAGE_READING 3
#define ADC_CURRENT_READING 5

void adc_setup (void) {
  adc_use_channels_6_7();
  adc_voltage_high();
  adc_free_running_interrupt_at_125kHz();    
}

void adc_use_channels_6_7 (void) {
  DIDR0 = _BV(ADC7D) | _BV(ADC6D);
}

void adc_free_running_interrupt_at_125kHz (void) {
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADATE) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);
}

void adc_current_high (void) {
  set_data_H_n_b(H_adc, ADC_CHANNEL_AND_REF, _BV(REFS0) | ADC_CURRENT_PORT);
  adc_effectuate_channel_and_ref();
}

void adc_current_low (void) {
  set_data_H_n_b(H_adc, ADC_CHANNEL_AND_REF, ADC_CURRENT_PORT);
  adc_effectuate_channel_and_ref();
}

void adc_voltage_high (void) {
  set_data_H_n_b(H_adc, ADC_CHANNEL_AND_REF, _BV(REFS0) | ADC_VOLTAGE_PORT);
  adc_effectuate_channel_and_ref();
}

void adc_voltage_low (void) {
  set_data_H_n_b(H_adc, ADC_CHANNEL_AND_REF, ADC_VOLTAGE_PORT);
  adc_effectuate_channel_and_ref();
}

void adc_effectuate_channel_and_ref (void) {
  ADMUX = get_b_data_H_n(H_adc, ADC_CHANNEL_AND_REF);
  set_data_H_n_b(H_adc, ADC_READINGS_TO_SKIP, 1);
}

ISR (ADC_vect) {
  uint8_t reading_l = ADCL;
  uint8_t reading_h = ADCH;
  uint8_t readings_to_skip = get_b_data_H_n(H_adc, ADC_READINGS_TO_SKIP);
  if (readings_to_skip > 0) {
    return set_data_H_n_b(H_adc, ADC_READINGS_TO_SKIP, readings_to_skip - 1);
  }
  uint16_t reading = reading_h << 8 | reading_l;
  uint8_t channel_and_ref = get_b_data_H_n(H_adc, ADC_CHANNEL_AND_REF);
  uint8_t channel = channel_and_ref & 0b111;
  if (channel == ADC_VOLTAGE_PORT) {
    set_data_H_n_w(H_adc, ADC_VOLTAGE_READING, reading);
    adc_current_high();
  }
  else if (channel == ADC_CURRENT_PORT) {
    set_data_H_n_w(H_adc, ADC_CURRENT_READING, reading);
    adc_voltage_high();
  }
  else {
    return set_status_H_S(H_adc, S_adc_unknown);
  }
  set_status_H_S(H_adc, S_processing_needed);
}

void h_adc () {
  cli();
  uint8_t status = get_status_H(H_adc);
  uint16_t current = get_w_data_H_n(H_adc, ADC_CURRENT_READING);
  uint16_t voltage = get_w_data_H_n(H_adc, ADC_VOLTAGE_READING);
  if (status == S_processing_needed) 
    set_status_H_S(H_adc, S_ok);
  sei();
  if (status != S_processing_needed) 
    return;
  if (current > 0x1FF)
    pin_on_D(D_CC_LED);
  else
    pin_off_D(D_CC_LED);
  if (voltage > 0x1FF)
    pin_on_D(D_CV_LED);
  else
    pin_off_D(D_CV_LED);
}
