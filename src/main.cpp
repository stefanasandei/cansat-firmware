#include <avr/io.h>
#include <util/delay.h>

#include <stdlib.h>

// dummy class to test embedded C++ usage
class Led {
public:
  Led() {
    DDRB |= (1 << DDB5);
    m_Data = static_cast<uint8_t *>(malloc(4 * sizeof(uint8_t)));
  }
  ~Led() {
    // sample usage of RAII
    free(m_Data);
  }

  void TurnOn() const { PORTB |= (1 << PORTB5); }
  void TurnOFF() const { PORTB &= ~(1 << PORTB5); }

private:
  uint8_t *m_Data;
};

int main() {
  Led l;

  for (;;) {
    l.TurnOn();
    _delay_ms(1000);
    l.TurnOFF();
    _delay_ms(1000);
  }

  return 0;
}
