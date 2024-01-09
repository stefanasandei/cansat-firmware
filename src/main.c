#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  DDRB |= (1 << DDB5); // Set digital pin 13 (PB5) as output

  while (1) {
    PORTB |= (1 << PORTB5);  // Turn on LED
    _delay_ms(1000);         // Delay for 1 second
    PORTB &= ~(1 << PORTB5); // Turn off LED
    _delay_ms(1000);         // Delay for 1 second
  }

  return 0;
}
