//
// Created by Stefan on 17/5/2024.
//

#ifndef APP_HPP
#define APP_HPP

#include "cansat.hpp"

#include "devices/accelerometer.hpp"
#include "devices/pressure.hpp"
#include "io/communicator.hpp"
#include "io/rom.hpp"

#include <avr/io.h>

namespace CanSat {

    enum ApplicationMode : uint8_t {
        Debug,
        Plot,
        Prod
    };

    struct SensorData {
        SpatialData spatial;
        int32_t temp, pressure;
    };

    class Application {
    public:
        Application();
        ~Application();

        void loop(ApplicationMode mode);

    private:
        void loop_debug();
        void loop_plot();
        void loop_prod();

    private:
        Communicator com, gps;

        Rom rom;

        Accelerometer acc;
        PressureSensor bmp;

        SensorData data;
        char msg[1024];
        int series_logged = 0;
        intptr_t curr_addr = 0;
    };

}// namespace CanSat

#endif//APP_HPP
