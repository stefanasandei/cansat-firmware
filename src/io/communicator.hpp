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
        Communicator(int recv, int trans);
        ~Communicator();

        void write(char *msg);

    private:
        void init() const;
        void wait() const;

    private:
        int m_RX, m_TX;
    };

}// namespace CanSat

#endif//COMMUNICATOR_HPP
