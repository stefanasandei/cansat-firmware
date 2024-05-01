//
// Created by Stefan on 4/4/2024.
//

#include "io/communicator.hpp"

#include <string.h>

namespace CanSat {

    Communicator::Communicator() {
        init();
    }

    Communicator::~Communicator() {}

    void Communicator::init() const {
        // High and low bits
        UBRR0H = (BUAD_RATE_CALC >> 8);
        UBRR0L = BUAD_RATE_CALC;

        // transimit and recieve enable
        UCSR0B = (1 << TXEN0) | (1 << TXCIE0) | (1 << RXEN0) | (1 << RXCIE0);
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);// 8 bit data format
    }

    void Communicator::wait() const {
        while ((UCSR0A & (1 << UDRE0)) == 0) {
        }
    }

    void Communicator::write(char *msg) {
        for (size_t i = 0; i < strlen(msg); i++) {
            wait();
            UDR0 = msg[i];
        }
    }

}// namespace CanSat
