//
// Created by Stefan on 4/4/2024.
//

#define DEBUG 0

#include "app.hpp"

int main(void) {
    CanSat::Application app;

    constexpr int PROD = false;
    int period = 0;

    for (;;) {
#if DEBUG
        app.loop(CanSat::ApplicationMode::Debug);
#else
        // after 3 mins
        if (period > 3 * 60 * 10 / 3 && PROD) {
            app.loop(CanSat::ApplicationMode::Prod);
        } else {
            app.loop(CanSat::ApplicationMode::Plot);
        }
#endif

        period++;
        _delay_ms(300);
    }

    return 0;
}
