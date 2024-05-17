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

    constexpr int MAX_SERIES = Rom::max_size() / sizeof(SensorData) * 2;
    constexpr bool PROD = false;

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
    }

    void Application::loop_debug() {
        (void) sprintf(msg, "(%ld,%ld,%ld);",
                       data.spatial.x, data.spatial.y, data.spatial.z);
        (void) sprintf(msg + strlen(msg), "(%ld);", data.temp);
        (void) sprintf(msg + strlen(msg), "(%ld);\n", data.pressure);
    }

    void Application::loop_plot() {
        (void) sprintf(msg, "%ld,%ld,%ld,%ld,%ld,\n", data.spatial.x, data.spatial.y, data.spatial.z, data.temp, data.pressure);
    }

    void Application::loop_prod() {
        if (series_logged > MAX_SERIES || curr_addr > 1000)
            return;

        if (!PROD)
            return;

        rom.write(curr_addr++, data.spatial.x);
        rom.write(curr_addr++, data.spatial.y);
        rom.write(curr_addr++, data.spatial.z);

        rom.write(curr_addr++, data.temp);
        rom.write(curr_addr++, data.pressure);

        series_logged++;
    }

}// namespace CanSat
