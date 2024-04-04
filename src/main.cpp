#include "io/communicator.hpp"

#include <util/delay.h>

const char msg[] = "sup bros\n";

int main(void) {
  CanSat::Communicator com;

  for (;;) {
    com.write(msg);
    _delay_ms(500);
  }

  return 0;
}
