#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define PIN_11 PD6

#define INLINE static __attribute__((always_inline)) inline

INLINE void portd_pin_as_output (uint8_t pin);
INLINE void portd_pin_on (uint8_t pin);
INLINE void portd_pin_off (uint8_t pin);
INLINE void setup_fast_timer();
INLINE void reset_time();
INLINE void clear_timer_on_compare_mode();
INLINE void match_every_1ms();
INLINE void scale_clock_down_by_1024();
INLINE void interupt_on_match();
INLINE uint32_t time_ms();

int main ()
{
    portd_pin_as_output(PIN_11);
    setup_fast_timer();
    sei();
    while (1)
    {
        uint32_t const ms = time_ms();
        if (ms % 1000 == 0) {
            portd_pin_on(PIN_11);
        }
        else if (ms % 100 == 0) {
            portd_pin_off(PIN_11);
        }
    }
    return 0;
}

void setup_fast_timer () {
    reset_time();
    scale_clock_down_by_1024();
    clear_timer_on_compare_mode();
    match_every_1ms();
    interupt_on_match();
}

void scale_clock_down_by_1024 () {
    TCCR0B |= _BV(CS02) | _BV(CS00);
}

void clear_timer_on_compare_mode () {
    TCCR0A |= _BV(WGM01);
}

void interupt_on_match () {
    TIMSK |= _BV(OCIE0A);
}

void match_every_1ms () {
    OCR0A = (F_CPU / 1024 / 1000) - 1;
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

volatile uint32_t IRQ__time_ms;

void reset_time () {
    IRQ__time_ms = 0;
}

uint32_t time_ms () {
    return IRQ__time_ms;
}

ISR (TIMER0_COMPA_vect) {
    ++IRQ__time_ms;
}
