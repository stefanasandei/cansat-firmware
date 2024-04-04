#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include "../cansat.hpp"

#include <avr/io.h>

namespace CanSat {

class Communicator {
public:
  Communicator();
  ~Communicator();

  void write(const char *msg);

private:
  void init() const;
  void wait() const;
};

} // namespace CanSat

#endif
