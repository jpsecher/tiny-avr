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
INLINE uint8_t usi_master_transfer ();
INLINE void usi_master_stop ();
INLINE void pin_as_output (uint8_t pin);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);
INLINE void usi_pin_release (uint8_t pin);
INLINE void usi_pin_yank (uint8_t pin);
INLINE void usi_pin_as_output (uint8_t pin);
INLINE void usi_pin_as_input (uint8_t pin);
INLINE void usi_set_data (uint8_t data);
INLINE void usi_reset_data_register ();
INLINE void usi_use_two_wire_software_clock ();
INLINE void usi_prepare_transmit_8_bit ();
INLINE void usi_prepare_transmit_1_bit ();
INLINE void usi_wait_for_scl_release ();
INLINE void usi_toggle_clock_line ();
INLINE void usi_write_byte (uint8_t data);
INLINE void usi_send_until_transfer_complete ();

INLINE void reset_error_state ();
INLINE uint8_t usi_fail_on_slave_nack ();
INLINE uint8_t usi_fail_on_missing_start ();
INLINE uint8_t usi_fail_on_missing_stop ();
INLINE uint8_t usi_fail_on_bus_noise ();

void show_error_state_perpetually ();
void usi_start_transceiver_with_data (uint8_t const * msg, uint8_t msgSize);

union
{
  struct
  {
    uint8_t usi_slave_did_not_respond : 1;
    uint8_t usi_missing_stop : 1;
    uint8_t usi_missing_start : 1;
    uint8_t usi_unexpected_start : 1;
    uint8_t usi_unexpected_stop : 1;
    uint8_t usi_unexpected_collision : 1;
  };
  uint8_t value;
}
error;

int main ()
{
  reset_error_state();
  master_initialise();
  pin_as_output(PIN_STATUS);
  while (1)
  {
    uint8_t const low[] = { 0b11000000, 0b01000000, 0x00, 0x00 };
    usi_start_transceiver_with_data(low, 4);
    if (error.value != 0)
      show_error_state_perpetually();
    pin_on(PIN_STATUS);
    _delay_ms(500);
    uint8_t const high[] = { 0b11000000, 0b01000000, 0xFF, 0xFF };
    usi_start_transceiver_with_data(high, 4);
    if (error.value != 0)
      show_error_state_perpetually();
    pin_off(PIN_STATUS);
    _delay_ms(200);
  }
  return 0;
}

void show_error_state_perpetually ()
{
  pin_as_output(PIN_STATUS);
  while (1)
  {
    pin_on(PIN_STATUS);
    _delay_ms(1000);
    pin_off(PIN_STATUS);
    _delay_ms(200);
    uint8_t error_bits = error.value;
    while (error_bits != 0)
    {
      pin_on(PIN_STATUS);
      _delay_ms(25);
      if (!(error_bits & 1))
        pin_off(PIN_STATUS);
      _delay_ms(275);
      pin_off(PIN_STATUS);
      _delay_ms(100);
      error_bits >>= 1;
    }
    pin_off(PIN_STATUS);
    _delay_ms(1000);
  }
}

void master_initialise ()
{
  usi_pin_release(PIN_USI_SDA);
  usi_pin_release(PIN_USI_SCL);
  usi_pin_as_output(PIN_USI_SCL);
  usi_pin_as_output(PIN_USI_SDA);
  usi_reset_data_register();
  usi_prepare_transmit_8_bit();
}

void usi_master_start ()
{
  usi_pin_yank(PIN_USI_SDA);
  _delay_us(I2C_SHORT_DELAY_US);
  usi_pin_yank(PIN_USI_SCL);
  usi_pin_release(PIN_USI_SDA);
}

void usi_start_transceiver_with_data (uint8_t const * msg, uint8_t msgSize)
{
  if (usi_fail_on_bus_noise())
    return;
  usi_pin_release(PIN_USI_SCL);
  usi_wait_for_scl_release();
  _delay_us(I2C_SHORT_DELAY_US);
  usi_master_start();
  // if (usi_fail_on_missing_start())
  //   return;
  do
  {
    usi_write_byte(*(msg++));
    if (usi_fail_on_slave_nack())
      return;
  }
  while (--msgSize);
  usi_master_stop();
  usi_fail_on_missing_stop();
}

void usi_send_until_transfer_complete ()
{
  do
  {
    _delay_us(I2C_LONG_DELAY_US);
    // Generate positve SCL edge.
    usi_toggle_clock_line();
    usi_wait_for_scl_release();
    _delay_us(I2C_SHORT_DELAY_US);
    // Generate negative SCL edge.
    usi_toggle_clock_line();
  }
  while (!(USISR & _BV(USIOIF)));
}

uint8_t usi_master_transfer ()
{
  usi_send_until_transfer_complete();
  _delay_us(I2C_LONG_DELAY_US);
  uint8_t received = USIDR;
  usi_reset_data_register();
  usi_pin_as_output(PIN_USI_SDA);
  return received;
}

void usi_master_stop ()
{
  usi_pin_yank(PIN_USI_SDA);
  usi_pin_release(PIN_USI_SCL);
  usi_wait_for_scl_release();
  _delay_us(I2C_SHORT_DELAY_US);
  usi_pin_release(PIN_USI_SDA);
  _delay_us(I2C_LONG_DELAY_US);
}

void pin_as_output (uint8_t pin)
{
  DDRB |= _BV(pin);
}

void pin_on (uint8_t pin)
{
  PORTB |= _BV(pin);
}

void pin_off (uint8_t pin)
{
  PORTB &= ~_BV(pin);
}

void reset_error_state ()
{
  error.value = 0;
}

void usi_pin_release (uint8_t pin)
{
  PORT_USI |= _BV(pin);
}

void usi_pin_yank (uint8_t pin)
{
  PORT_USI &= ~(_BV(pin));
}

void usi_pin_as_output (uint8_t pin)
{
  DDR_USI |= _BV(pin);
}

void usi_pin_as_input (uint8_t pin)
{
  DDR_USI &= ~(_BV(pin));
}

void usi_set_data (uint8_t data)
{
  USIDR = data;
}

void usi_reset_data_register ()
{
  usi_set_data(0xFF);
}

void usi_use_two_wire_software_clock ()
{
  USICR = _BV(USIWM1) | _BV(USICS1) | _BV(USICLK);
}

void usi_prepare_transmit_8_bit ()
{
  USISR = _BV(USISIF) | _BV(USIOIF) | _BV(USIPF) | _BV(USIDC);
}

void usi_prepare_transmit_1_bit ()
{
  usi_prepare_transmit_8_bit();
  // Only count for one clock cycle (ie. change twice).
  USISR |= (0xE<<USICNT0);
}

void usi_wait_for_scl_release ()
{
  while (!(PIN_USI & _BV(PIN_USI_SCL)));
}

void usi_toggle_clock_line ()
{
  USICR = _BV(USIWM1) | _BV(USICS1) | _BV(USICLK) | _BV(USITC);
}

void usi_write_byte (uint8_t data)
{
  usi_pin_yank(PIN_USI_SCL);
  USIDR = data;
  usi_prepare_transmit_8_bit();
  usi_master_transfer();
}

uint8_t usi_fail_on_slave_nack ()
{
  usi_pin_as_input(PIN_USI_SDA);
  usi_prepare_transmit_1_bit();
  #define I2C_NACK 0x1
  if (usi_master_transfer() & I2C_NACK)
    error.usi_slave_did_not_respond = 1;
  return error.value;
}

uint8_t usi_fail_on_missing_start ()
{
  if (!(USISR & _BV(USISIF)))
    error.usi_missing_start = 1;
  return error.value;
}

uint8_t usi_fail_on_missing_stop ()
{
  if (!(USISR & _BV(USIPF)))
    error.usi_missing_stop = 1;
  return error.value;
}

uint8_t usi_fail_on_bus_noise ()
{
  if (USISR & _BV(USISIF))
    error.usi_unexpected_start = 1;
  // if (USISR & _BV(USIPF))
  //   error.usi_unexpected_stop = 1;
  if (USISR & _BV(USIDC))
    error.usi_unexpected_collision = 1;
  return error.value;
}
