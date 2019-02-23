#include <avr/io.h>
#include <util/delay.h>

#if defined(__AVR_ATtiny2313A__)
#  define DDR_USI DDRB
#  define PORT_USI PORTB
#  define PIN_USI PINB
#  define PORT_USI_SDA PORTB5
#  define PORT_USI_SCL PORTB7
#  define PIN_USI_SDA PINB5
#  define PIN_USI_SCL PINB7
#endif

#define T2_TWI    ((F_CPU * 1.3) /1000000) +1 // >1,3us
#define T4_TWI    ((F_CPU * 0.6) /1000000) +1 // >0,6us

#define INLINE static inline

INLINE void master_initialise ();
INLINE uint8_t start_transceiver_with_data (uint8_t const * msg, uint8_t msgSize);
INLINE uint8_t master_transfer ();
INLINE uint8_t master_stop ();
INLINE void pin_as_output (uint8_t pin);
INLINE void pin_on (uint8_t pin);
INLINE void pin_off (uint8_t pin);

int main () {
  master_initialise();
  pin_as_output(PB0);
  while (1) {
    uint8_t const low[] = { 0b11000000, 0b01000000, 0x00, 0x00 };
    start_transceiver_with_data(low, 4);
    pin_on(PB0);
    _delay_ms(500);
    uint8_t const high[] = { 0b11000000, 0b01000000, 0xFF, 0xFF };
    start_transceiver_with_data(high, 4);
    pin_off(PB0);
    _delay_ms(500);
  }
  return 0;
}

void usi_pin_pullup (uint8_t pin) {
  PORT_USI |= _BV(pin);
}

void usi_pin_as_output (uint8_t pin) {
  DDR_USI |= _BV(pin);
}

void usi_set_data (uint8_t data) {
  USIDR = data;
}

void usi_release_sda () {
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
  usi_pin_pullup(PIN_USI_SDA);
  usi_pin_pullup(PIN_USI_SCL);
  usi_pin_as_output(PIN_USI_SCL);
  usi_pin_as_output(PIN_USI_SDA);
  usi_release_sda();
  usi_prepare_transmit_8_bit();
}

uint8_t start_transceiver_with_data (uint8_t const * msg, uint8_t msgSize) {
  /* Release SCL to ensure that (repeated) Start can be performed */
  PORT_USI |= (1<<PIN_USI_SCL);                     // Release SCL.
  while( !(PIN_USI & (1<<PIN_USI_SCL)) );          // Verify that SCL becomes high.
  _delay_us( T4_TWI/4 );                         // Delay for T4TWI if TWI_FAST_MODE
  /* Generate Start Condition */
  PORT_USI &= ~(1<<PIN_USI_SDA);                    // Force SDA LOW.
  _delay_us( T4_TWI/4 );
  PORT_USI &= ~(1<<PIN_USI_SCL);                    // Pull SCL LOW.
  PORT_USI |= (1<<PIN_USI_SDA);                     // Release SDA.
  do {
    /* Write a byte */
    PORT_USI &= ~(1<<PIN_USI_SCL);                // Pull SCL LOW.
    USIDR     = *(msg++);                        // Setup data.
    usi_prepare_transmit_8_bit();
    master_transfer();
    /* Clock and verify (N)ACK from slave */
    DDR_USI  &= ~(1<<PIN_USI_SDA);                // Enable SDA as input.
    #define TWI_NACK_BIT  0       // Bit position for (N)ACK bit.
    usi_prepare_transmit_1_bit();
    if( master_transfer() & (1<<TWI_NACK_BIT) )
    {
      // Slave did not respond.
      // TODO: return 0;
    }
  } while( --msgSize);                             // Until all data sent/received.
  master_stop();                           // Send a STOP condition on the TWI bus.
  return 1;
}

uint8_t master_transfer () {
  uint8_t temp;
  temp  =  (0<<USISIE)|(0<<USIOIE)|                 // Interrupts disabled
           (1<<USIWM1)|(0<<USIWM0)|                 // Set USI in Two-wire mode.
           (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|     // Software clock strobe as source.
           (1<<USITC);                              // Toggle Clock Port.
  do {
    _delay_us( T2_TWI/4 );
    USICR = temp;                          // Generate positve SCL edge.
    while( !(PIN_USI & (1<<PIN_USI_SCL)) );// Wait for SCL to go high.
    _delay_us( T4_TWI/4 );
    USICR = temp;                          // Generate negative SCL edge.
  } while( !(USISR & (1<<USIOIF)) );       // Check for transfer complete.
  _delay_us( T2_TWI/4 );
  temp  = USIDR;                           // Read out data.
  USIDR = 0xFF;                            // Release SDA.
  DDR_USI |= (1<<PIN_USI_SDA);             // Enable SDA as output.
  return temp;                             // Return the data from the USIDR
}

uint8_t master_stop () {
  PORT_USI &= ~(1<<PIN_USI_SDA);           // Pull SDA low.
  PORT_USI |= (1<<PIN_USI_SCL);            // Release SCL.
  while( !(PIN_USI & (1<<PIN_USI_SCL)) );  // Wait for SCL to go high.
  _delay_us( T4_TWI/4 );
  PORT_USI |= (1<<PIN_USI_SDA);            // Release SDA.
  _delay_us( T2_TWI/4 );
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
