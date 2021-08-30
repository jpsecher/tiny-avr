#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define PIN_9 PD5 // PCINT16
#define PIN_11 PD6
#define PIN_14 PB2

#define INLINE static __attribute__((always_inline)) inline

INLINE void portd_pin_as_input (uint8_t pin);
INLINE void portd_pin_as_output (uint8_t pin);
INLINE void portd_pin_on (uint8_t pin);
INLINE void portd_pin_off (uint8_t pin);
INLINE void portb_pin_as_output (uint8_t pin);
INLINE void portb_pin_on (uint8_t pin);
INLINE void portb_pin_off (uint8_t pin);

void setup_irq_on_push_button();
void pcint_17_to_11_generate_interrupt_on_pcint2();
void enable_interrupt_from_button();

struct {
    uint8_t button_pressed : 1;
} state;

int main ()
{
    portb_pin_as_output(PIN_14);
    portd_pin_as_output(PIN_11);
    portd_pin_as_input(PIN_9);
    setup_irq_on_push_button();
    while (1)
    {
        if (state.button_pressed) {
            portd_pin_on(PIN_11);
        }
        else {
            portd_pin_off(PIN_11);
        }
        static uint8_t toggle = 0;
        if (toggle)
            portb_pin_on(PIN_14);
        else
            portb_pin_off(PIN_14);
        toggle = !toggle;
        _delay_ms(100);
    }
    return 0;
}

void setup_irq_on_push_button () {
    pcint_17_to_11_generate_interrupt_on_pcint2();
    enable_interrupt_from_button();
    sei();
}

void pcint_17_to_11_generate_interrupt_on_pcint2 () {
    GIMSK |= _BV(PCIE2);
}

void enable_interrupt_from_button () {
    PCMSK2 |= _BV(PCINT16);
}

ISR (PCINT2_vect) {
    // IO pulled down means button pressed.
    state.button_pressed = bit_is_clear(PIND, PIN_9) ? 1 : 0;
}

void portd_pin_as_input (uint8_t pin)
{
    PORTD |= _BV(pin);
}

void portd_pin_as_output (uint8_t pin)
{
    DDRD |= _BV(pin);
}

void portd_pin_on (uint8_t pin)
{
    PORTD |= _BV(pin);
}

void portd_pin_off (uint8_t pin)
{
    PORTD &= ~(_BV(pin));
}

void portb_pin_as_output (uint8_t pin)
{
    DDRB |= _BV(pin);
}

void portb_pin_on (uint8_t pin)
{
    PORTB |= _BV(pin);
}

void portb_pin_off (uint8_t pin)
{
    PORTB &= ~(_BV(pin));
}
