//
// Created by Stefan on 4/4/2024.
//

#include "devices/accelerometer.hpp"
#include "devices/pressure.hpp"
#include "io/communicator.hpp"
#include "io/rom.hpp"

#include <stdio.h>
#include <string.h>

#include <avr/eeprom.h>
#include <util/delay.h>

char msg[1024];

int main(void) {
    CanSat::Communicator com(0, 1);
    CanSat::Accelerometer acc;
    CanSat::PressureSensor bmp;

    CanSat::Rom rom;

    CanSat::SpatialData spatial{};
    int32_t temp, pressure;

    // rom.write(420, 69);

    for (;;) {
        acc.get_rotation(spatial);
        acc.get_temp(temp);
        bmp.get_pressure(pressure);

        (void) sprintf(msg, "(%ld,%ld,%ld);",
                       spatial.x, spatial.y, spatial.z);
        (void) sprintf(msg + strlen(msg), "(%ld);", temp);
        (void) sprintf(msg + strlen(msg), "(%ld);\n", pressure);

        // (void) sprintf(msg, "read %d\n", rom.read(420));

        com.write(msg);

        _delay_ms(300);
    }

    return 0;
}
