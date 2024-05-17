//
// Created by Stefan on 4/4/2024.
//

#include "io/communicator.hpp"

#include <string.h>

namespace CanSat {

    Communicator::Communicator(int recv, int trans) : m_RX(recv), m_TX(trans) {
        init();
    }

    Communicator::~Communicator() {}

    void Communicator::init() const {
        // Set RX and TX pins as inputs/outputs
        DDRD |= (1 << m_TX); // Set TX pin as output
        DDRD &= ~(1 << m_RX);// Set RX pin as input

        // Set baud rate
        unsigned int ubrr = F_CPU / 16 / BUAD - 1;
        UBRR0H = (unsigned char) (ubrr >> 8);
        UBRR0L = (unsigned char) ubrr;

        // Enable receiver and transmitter
        UCSR0B = (1 << TXEN0) | (1 << RXEN0);
        // Set frame format: 8data, 1stop bit
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
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
