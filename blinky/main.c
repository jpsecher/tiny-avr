#include <avr/io.h>
#include <util/delay.h>

int main () {
  // set PB4 to be output
	DDRB = 0b00010000;
  while (1) {
    PORTB = 0b00010000;
    _delay_ms(500);
    PORTB = 0b00000000;
    _delay_ms(500);
  }
  return 0;
}
