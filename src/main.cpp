//
// Created by Stefan on 4/4/2024.
//

#include "devices/accelerometer.hpp"
#include "io/communicator.hpp"

#include <stdio.h>

#include <util/delay.h>

char msg[1024];

int main(void) {
    CanSat::Communicator com;
    CanSat::Accelerometer acc;

    CanSat::SpatialData spatial{};

    for (;;) {
        acc.get_rotation(spatial);

        (void) sprintf(msg, "(%d, %d, %d)\n",
                       spatial.x, spatial.y, spatial.z);
        com.write(msg);

        _delay_ms(500);
    }

    return 0;
}
