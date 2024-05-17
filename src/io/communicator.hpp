//
// Created by Stefan on 4/4/2024.
//

#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include "cansat.hpp"

#include <avr/io.h>

namespace CanSat {

    class Communicator {
    public:
        Communicator(int recv, int trans, int baud = BUAD);
        ~Communicator();

        void write(char *msg);
        char *read();

    private:
        void init() const;
        void wait() const;

    private:
        int m_RX, m_TX;
        static const int BUFFER_SIZE = 128;// Define a buffer size
        char m_buffer[BUFFER_SIZE];        // Buffer for storing received data
    };

}// namespace CanSat

#endif//COMMUNICATOR_HPP
