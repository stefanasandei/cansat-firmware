//
// Created by Stefan on 4/4/2024.
//

#include "devices/accelerometer.hpp"
#include "io/communicator.hpp"

#include <stdio.h>
#include <string.h>

#include <avr/eeprom.h>
#include <util/delay.h>

char msg[1024];

int main(void) {
    CanSat::Communicator com(0, 1);
    CanSat::Accelerometer acc;

    CanSat::SpatialData spatial{};
    int32_t temp;

    // from 0 to E2END
    uint8_t val = 0, *addr = 0;
    eeprom_write_byte(addr, 3);
    val = eeprom_read_byte(addr);

    for (;;) {
        acc.get_rotation(spatial);
        acc.get_temp(temp);

        (void) sprintf(msg, "(%ld,%ld,%ld);",
                       spatial.x, spatial.y, spatial.z);
        (void) sprintf(msg + strlen(msg), "(%ld);\n", temp);

        com.write(msg);

        _delay_ms(300);
    }

    return 0;
}
