//
// Created by Stefan on 17/5/2024.
//

#include "app.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

namespace CanSat {

    Application::Application() : com(0, 1), gps(4, 3, 9600) {}

    Application::~Application() {}

    void Application::loop(ApplicationMode mode) {
        acc.get_rotation(data.spatial);
        acc.get_temp(data.temp);
        bmp.get_pressure(data.pressure);

        switch (mode) {
            case ApplicationMode::Debug:
                loop_debug();
                break;

            case ApplicationMode::Plot:
                loop_plot();
                break;

            case ApplicationMode::Prod:
                loop_prod();
                break;

            default:
                break;
        }

        com.write(msg);
        _delay_ms(300);
    }

    void Application::loop_debug() {
        (void) sprintf(msg, "(%ld,%ld,%ld);",
                       data.spatial.x, data.spatial.y, data.spatial.z);
        (void) sprintf(msg + strlen(msg), "(%ld);", data.temp);
        (void) sprintf(msg + strlen(msg), "(%ld);\n", data.pressure);
    }

    void Application::loop_plot() {
        (void) sprintf(msg, "%ld,%ld,%ld,%ld,%ld,%ld,\n", data.spatial.x, data.spatial.y, data.spatial.z, data.temp, data.pressure);
    }

    void Application::loop_prod() {}

}// namespace CanSat
