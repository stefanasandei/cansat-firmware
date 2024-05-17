//
// Created by Stefan on 4/4/2024.
//

#include "io/rom.hpp"

namespace CanSat {

    Rom::Rom() {
        m_Begin = 0;
        m_End = E2END;
    }

    Rom::~Rom() {}

    void Rom::clear() const {}

    void Rom::write(intptr_t addr, uint8_t value) const {
        if (out_of_bounds(addr)) return;

        eeprom_update_byte((uint8_t *) (addr), value);
    }

    uint8_t Rom::read(intptr_t addr) const {
        if (out_of_bounds(addr)) return 0;

        return eeprom_read_byte((uint8_t *) (addr));
    }

    bool Rom::out_of_bounds(intptr_t addr) const {
        return addr < m_Begin || addr > m_End;
    }


}// namespace CanSat
