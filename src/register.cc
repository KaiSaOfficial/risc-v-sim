#include "cassert"
#include "cstdint"
#include "cstdio"
#include "cstring"
#include <cassert>
#include <cstdint>
#include <cstring>

#include "register.h"

Register::Register(void) {
    this->reg = new uint32_t[DEPTH];
std:
    memset((void *)(this->reg), 0x00000000, this->DEPTH);
};

uint32_t Register::read(uint8_t reg_n) const { return *(this->reg + reg_n); }

void Register::write(const uint32_t Value, uint8_t reg_n) {
    *(this->reg + reg_n) = Value;
}

void Register::debug(uint8_t reg_n) const {
    std::printf("Reg: %d, %x\n", reg_n, *(this->reg + reg_n));
}

Register::~Register(void) { delete[] reg; };