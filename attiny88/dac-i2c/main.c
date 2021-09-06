//
// Connections
// ===========
//
// All buttons & encoder use PCI0 (pins PBx).
// LEDs on PDx.
// DAC on PC4/5.


//  PD6 --- +LED(R)- --- GND
#define D_CC_LED PD6

//  PD7 --- +LED(G)- --- GND
#define D_CV_LED PD7

// PB2 --- SW_PUSH --- GND
// PD2 --- SW_PUSH --- GND
#define B_OE_SW PB2

// PC4 --- SDA:MCP4725
// PC5 --- SCL:MCP4725
#define C_SDA PC4
#define C_SCL PC5
#define DAC_ADDR 0b1100000


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
  // TWI errors.
  S_twi_idle_intr = 4,
  S_twi_start_failed = 5,
  S_twi_unknown = 6,
  S_twi_slave_not_ready = 7,
};

enum Intr_Handler {
  H_sanity,
  H_button,
  H_twi,
};

// Has to match the number of interrupt handlers listed above.
#define N_INTR_HANDLERS 3
uint8_t intr_handler_data_layout_H [N_INTR_HANDLERS];
uint8_t intr_handler_to_data_index_H [N_INTR_HANDLERS];

// Has to match (or exceed) number of bytes reserved for all interrupt handlers.
#define N_INTR_TOTAL_BYTES 11
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
INLINE uint8_t get_status_H (uint8_t handler);
INLINE uint8_t get_data_H_n (uint8_t handler, uint8_t index);
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
INLINE void pcint_7_to_0_generate_interrupt_on_pci0 (void);
INLINE void enable_interrupt_from_buttons (void);
INLINE void init_sanity_handler (uint8_t handler);
INLINE void init_button_handler (uint8_t handler);
INLINE void init_twi_handler (uint8_t handler);
INLINE void setup_twi_normal_speed_master_transfer (void);
INLINE void twi_start (void);
INLINE void twi_addr_w (void);
INLINE return_t twi_send_8_bit (uint8_t slave, uint8_t value);
INLINE return_t twi_send_16_bit (uint8_t slave, uint16_t value);
INLINE return_t twi_send_32_bit (uint8_t slave, uint32_t value);
INLINE void twi_first_byte (void);
INLINE void twi_next_byte (void);
INLINE void twi_stop (void);
INLINE void twi_acknowledge_interrupt (void);

void twi_abort_with_error (uint8_t error);

// Functions tied to specific Intr_Handlers.
void unknown_error (void);
void h_button (void);
void h_twi (void);

# define TWI_MAX_BYTES_N 4
# define TWI_STATE 1
# define TWI_SLAVE_ADDR 2
# define TWI_BYTES_N 3
# define TWI_BYTE_ARRAY 4


int main () {
  init_intr_handler_tables();
  ui_init();
  setup_irq_on_button();
  setup_twi_normal_speed_master_transfer();
  sanity_check();
  sei();
  while (1) {
    while (error_present()) {
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
  init_twi_handler(H_twi);
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

void init_twi_handler (uint8_t handler) {
  // Status byte, state, slave address, bytes to send, and array of 4 bytes for data.
  intr_handler_data_layout_H[handler] = 8;
  intr_handler_H[handler] = h_twi;
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

uint8_t get_status_H (uint8_t handler) {
  return get_data_H_n(handler, 0);
}

uint8_t get_data_H_n (uint8_t handler, uint8_t index) {
  return intr_handler_data[intr_handler_to_data_index_H[handler]+index];
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
  state = get_data_H_n(H_button, 1);
  set_status_H_S(H_button, S_ok);
  set_data_H_n_b(H_button, 1, 0x00);
  sei();
  if (status == S_processing_needed) {
    if (state) {
      // Set DAC output to 0V.
      twi_send_16_bit(DAC_ADDR, 0);
    }
  }
}


//
// TWI
//

enum Twi_State {
  // Values have to match index in data array:
  TS_sending_0,
  TS_sending_1,
  TS_sending_2,
  TS_sending_3,
  // The remaining values are unimportant:
  TS_addressing,
  TS_starting,
  TS_idle,
};

ISR (TWI_vect) {
  uint8_t status = TWSR & 0b11111000;
# define TWI_STATUS_STARTED 0x08
# define TWI_STATUS_SLAVE_W_ACK 0x18
# define TWI_STATUS_SLAVE_W_NACK 0x20
# define TWI_STATUS_DATA_ACK 0x28
# define TWI_STATUS_DATA_NACK 0x30
# define TWI_STATUS_SLAVE_LOST 0x38
  switch (get_data_H_n(H_twi, TWI_STATE)) {
    case TS_idle:
      return twi_acknowledge_interrupt();
    case TS_starting:
      if (status != TWI_STATUS_STARTED)
        return twi_abort_with_error(S_twi_start_failed);
      return twi_addr_w();
    case TS_addressing:
      if (status != TWI_STATUS_SLAVE_W_ACK)
        return twi_abort_with_error(S_twi_slave_not_ready);
      return twi_first_byte();
  }
  // All other sending states behave the same:
  if (status != TWI_STATUS_DATA_ACK)
    return twi_abort_with_error(S_twi_slave_not_ready);
  return twi_next_byte();
}

void twi_abort_with_error (uint8_t error) {
  set_status_H_S(H_twi, error);
  twi_stop();
}

void h_twi () {
  // TODO:
}

return_t twi_send_8_bit (uint8_t slave, uint8_t value) {
  if (get_data_H_n(H_twi, TWI_STATE) != TS_idle)
    return BUSY;
  set_data_H_n_b(H_twi, TWI_SLAVE_ADDR, slave);
  set_data_H_n_b(H_twi, TWI_BYTES_N, 1);
  set_data_H_n_b(H_twi, TWI_BYTE_ARRAY, value);
  twi_start();
  return SUCCESS;
}

return_t twi_send_16_bit (uint8_t slave, uint16_t value) {
  if (get_data_H_n(H_twi, TWI_STATE) != TS_idle)
    return BUSY;
  set_data_H_n_b(H_twi, TWI_SLAVE_ADDR, slave);
  set_data_H_n_b(H_twi, TWI_BYTES_N, 2);
  set_data_H_n_b(H_twi, TWI_BYTE_ARRAY, (value >> 8));
  set_data_H_n_b(H_twi, TWI_BYTE_ARRAY+1, (value & 0xFF));
  twi_start();
  return SUCCESS;
}

return_t twi_send_32_bit (uint8_t slave, uint32_t value) {
  if (get_data_H_n(H_twi, TWI_STATE) != TS_idle)
    return BUSY;
  set_data_H_n_b(H_twi, TWI_SLAVE_ADDR, slave);
  set_data_H_n_b(H_twi, TWI_BYTES_N, 4);
  set_data_H_n_b(H_twi, TWI_BYTE_ARRAY, (value >> 24));
  set_data_H_n_b(H_twi, TWI_BYTE_ARRAY+1, ((value >> 16) & 0xFF));
  set_data_H_n_b(H_twi, TWI_BYTE_ARRAY+2, ((value >> 8) & 0xFF));
  set_data_H_n_b(H_twi, TWI_BYTE_ARRAY+3, (value & 0xFF));
  twi_start();
  return SUCCESS;
}

void twi_start (void) {
  set_data_H_n_b(H_twi, TWI_STATE, TS_starting);
  // Start & use interrupts.
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE);
}

void twi_addr_w (void) {
  set_data_H_n_b(H_twi, TWI_STATE, TS_addressing);
  TWDR = get_data_H_n(H_twi, TWI_SLAVE_ADDR) << 1;  // Bit 0 means R/~W
  twi_acknowledge_interrupt();
}

void twi_first_byte (void) {
  uint8_t bytes_total = get_data_H_n(H_twi, TWI_BYTES_N);
  if (bytes_total == 0) {
    return twi_stop();
  }
  set_data_H_n_b(H_twi, TWI_STATE, TS_sending_0);
  TWDR = get_data_H_n(H_twi, TWI_BYTE_ARRAY);
  twi_acknowledge_interrupt();
}

void twi_next_byte (void) {
  uint8_t bytes_total = get_data_H_n(H_twi, TWI_BYTES_N);
  uint8_t next_index = get_data_H_n(H_twi, TWI_STATE) + 1;
  if (next_index >= bytes_total)
    return twi_stop();
  set_data_H_n_b(H_twi, TWI_STATE, next_index);
  TWDR = get_data_H_n(H_twi, TWI_BYTE_ARRAY+next_index);
  twi_acknowledge_interrupt();
}

void twi_stop (void) {
  set_data_H_n_b(H_twi, TWI_STATE, TS_idle);
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
}

void twi_acknowledge_interrupt (void) {
  TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
}

void setup_twi_normal_speed_master_transfer (void) {
  // f_SCL = 8MHz / (16 + (2 * 10 * 4)) ~= 83kHz
  TWBR = 10;  // Divide by 10
  TWSR |= 0b01;  // Divide by 4
  // Start idle.
  set_data_H_n_b(H_twi, TWI_STATE, TS_idle);
  // Take control of pins.
  TWCR = _BV(TWEN);
}

//
// DAC
//

