#include <avr/io.h>
#include <util/delay.h>

#if defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny4313__)
#  define DDR_USI DDRB
#  define PORT_USI PORTB
#  define PIN_USI PINB
#  define PORT_USI_SDA PORTB5
#  define PORT_USI_SCL PORTB7
#  define PIN_USI_SDA PINB5
#  define PIN_USI_SCL PINB7
#endif

#define PIN_STATUS PB0

// TWI fast mode.
#define I2C_LONG_DELAY_US ((((F_CPU * 1.3) /1000000) +1) / 4)
#define I2C_SHORT_DELAY_US ((((F_CPU * 0.6) /1000000) +1) / 4)

#define INLINE static inline

INLINE void master_initialise ();
INLINE void usi_start_transceiver_with_data (uint8_t const * msg, uint8_t msgSize);
INLINE uint8_t usi_master_transfer ();
INLINE uint8_t usi_master_stop ();
INLINE void pin_as_output (uint8_t pin);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

union {
  uint8_t value;
  struct {
    uint8_t usi_slave_did_not_respond : 1;
    uint8_t usi_sda_not_clear : 1;
    uint8_t usi_sck_not_clear : 1;
  };
} error;

void reset_error_state () {
  error.value = 0;
  // error.usi_sda_not_clear = 1;
  // error.usi_sck_not_clear = 1;
}

void show_error_state_perpetually () {
  pin_as_output(PIN_STATUS);
  while (1) {
    pin_on(PIN_STATUS);
    _delay_ms(1000);
    pin_off(PIN_STATUS);
    _delay_ms(200);
    uint8_t error_bits = error.value;
    while (error_bits != 0) {
      if (error_bits & 1) {
        pin_on(PIN_STATUS);
      }
      _delay_ms(200);
      pin_off(PIN_STATUS);
      _delay_ms(200);
      error_bits >>= 1;
    }
    pin_off(PIN_STATUS);
    _delay_ms(1000);
  }
}

int main () {
  reset_error_state();
  master_initialise();
  pin_as_output(PIN_STATUS);
  while (1) {
    uint8_t const low[] = { 0b11000000, 0b01000000, 0x00, 0x00 };
    usi_start_transceiver_with_data(low, 4);
    if (error.value != 0) show_error_state_perpetually();
    pin_on(PIN_STATUS);
    _delay_ms(500);
    uint8_t const high[] = { 0b11000000, 0b01000000, 0xFF, 0xFF };
    usi_start_transceiver_with_data(high, 4);
    if (error.value != 0) show_error_state_perpetually();
    pin_off(PIN_STATUS);
    _delay_ms(200);
  }
  return 0;
}

void usi_pin_release (uint8_t pin) {
  PORT_USI |= _BV(pin);
}

void usi_pin_yank (uint8_t pin) {
  PORT_USI &= ~(_BV(pin));
}

void usi_pin_as_output (uint8_t pin) {
  DDR_USI |= _BV(pin);
}

void usi_set_data (uint8_t data) {
  USIDR = data;
}

void usi_release_data_register () {
  usi_set_data(0xFF);
}

void usi_use_two_wire_software_clock () {
  USICR = _BV(USIWM1) | _BV(USICS1) | _BV(USICLK);
}

void usi_prepare_transmit_8_bit () {
  USISR = _BV(USISIF) | _BV(USIOIF) | _BV(USIPF) | _BV(USIDC);
}

void usi_prepare_transmit_1_bit () {
  usi_prepare_transmit_8_bit();
  // Only count two twice.
  USISR |= (0xE<<USICNT0);
}

void master_initialise () {
  usi_pin_release(PIN_USI_SDA);
  usi_pin_release(PIN_USI_SCL);
  usi_pin_as_output(PIN_USI_SCL);
  usi_pin_as_output(PIN_USI_SDA);
  usi_release_data_register();
  usi_prepare_transmit_8_bit();
}

void usi_start_transceiver_with_data (uint8_t const * msg, uint8_t msgSize) {
  /* Release SCL to ensure that (repeated) Start can be performed */
  usi_pin_release(PIN_USI_SCL);
  while( !(PIN_USI & (1<<PIN_USI_SCL)) );          // Verify that SCL becomes high.
  _delay_us(I2C_SHORT_DELAY_US);
  /* Generate Start Condition */
  usi_pin_yank(PIN_USI_SDA);
  _delay_us(I2C_SHORT_DELAY_US);
  usi_pin_yank(PIN_USI_SCL);
  usi_pin_release(PIN_USI_SDA);
  do {
    /* Write a byte */
    usi_pin_yank(PIN_USI_SCL);
    USIDR     = *(msg++);                        // Setup data.
    usi_prepare_transmit_8_bit();
    usi_master_transfer();
    /* Clock and verify (N)ACK from slave */
    usi_pin_as_output(PIN_USI_SDA);
    #define TWI_NACK_BIT  0       // Bit position for (N)ACK bit.
    usi_prepare_transmit_1_bit();
    if( usi_master_transfer() & (1<<TWI_NACK_BIT) )
    {
      // Slave did not respond.
      error.usi_slave_did_not_respond = 1;
      return;
    }
  } while( --msgSize);                             // Until all data sent/received.
  usi_master_stop();                           // Send a STOP condition on the TWI bus.
  return;
}

void usi_toggle_clock_line () {
  USICR = _BV(USIWM1) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
}

uint8_t usi_master_transfer () {
  do {
    _delay_us(I2C_LONG_DELAY_US);
    // Generate positve SCL edge.
    usi_toggle_clock_line();
    while( !(PIN_USI & (1<<PIN_USI_SCL)) );// Wait for SCL to go high.
    _delay_us(I2C_SHORT_DELAY_US);
    // Generate negative SCL edge.
    usi_toggle_clock_line();
  } while( !(USISR & (1<<USIOIF)) );       // Check for transfer complete.
  _delay_us(I2C_LONG_DELAY_US);
  uint8_t received = USIDR;
  usi_release_data_register();
  usi_pin_as_output(PIN_USI_SDA);
  return received;
}

uint8_t usi_master_stop () {
  usi_pin_yank(PIN_USI_SDA);
  usi_pin_release(PIN_USI_SCL);
  while( !(PIN_USI & (1<<PIN_USI_SCL)) );  // Wait for SCL to go high.
  _delay_us(I2C_SHORT_DELAY_US);
  usi_pin_release(PIN_USI_SDA);
  _delay_us(I2C_LONG_DELAY_US);
# ifdef SIGNAL_VERIFY
  if( !(USISR & (1<<USIPF)) )
  {
    USI_TWI_state.errorState = USI_TWI_MISSING_STOP_CON;
    return 0;
  }
# endif
  return 1;
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
