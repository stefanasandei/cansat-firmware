//
// Created by Stefan on 17/5/2024.
//

#ifndef ROM_HPP
#define ROM_HPP

#include "cansat.hpp"

#include <avr/eeprom.h>
#include <avr/io.h>

namespace CanSat {

    class Rom {
    public:
        Rom();
        ~Rom();

        void clear() const;

        void write(intptr_t addr, uint8_t value) const;
        uint8_t read(intptr_t addr) const;

    private:
        bool out_of_bounds(intptr_t addr) const;

    private:
        int m_Begin, m_End;
    };

}// namespace CanSat

#endif//COMMUNICATOR_HPP
