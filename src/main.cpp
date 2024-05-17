//
// Created by Stefan on 4/4/2024.
//

#define DEBUG 1

#include "app.hpp"

int main(void) {
    CanSat::Application app;

    for (;;) {
#if DEBUG
        app.loop(CanSat::ApplicationMode::Debug);
#else
        app.loop(CanSat::ApplicationMode::Plot);
#endif
    }

    return 0;
}
