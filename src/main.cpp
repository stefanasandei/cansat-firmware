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
    int32_t temp;

    for (;;) {
        acc.get_rotation(spatial);
        acc.get_temp(temp);

        // (void) sprintf(msg, "%ld,%ld,%ld\n",
        //                spatial.x, spatial.y, spatial.z);
        (void) sprintf(msg, "%d\n", temp);
        com.write(msg);

        _delay_ms(100);
    }

    return 0;
}
