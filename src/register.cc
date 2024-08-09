#include <cassert>
#include <cstdint>
#include <cstring>
#include "debug.h"

#include "register.h"

// RISCV 32IM Reg Name
const char *regs[] = {"$0", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                      "s0", "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                      "a6", "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                      "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

Register::Register(void) {
    this->reg = new uint32_t[REG_N];
    assert(this->reg != nullptr);

    memset((void *)(this->reg), 0x00000000, this->REG_N);
    Log("physical reg area [%p, %p]", this->reg, this->reg + this->REG_N);
};

Register::~Register(void) { delete[] reg; };

uint32_t Register::read(uint8_t reg_n) const { return *(this->reg + reg_n); }

void Register::write(const uint32_t Value, uint8_t reg_n) {
    *(this->reg + reg_n) = Value;
}

void Register::reg_display() const {
    for (uint8_t i = 0; i < this->REG_N; i++) {
        if ((i == 1) || (i == 2) || (i == 3) || (i == 8) || (i == 9)) {
            Debug("%-10s\t0x%-10x\t0x%x", *(regs + i), *(this->reg + i),
                  *(this->reg + i));
        } else {
            Debug("%-10s\t0x%-10x\t%u", *(regs + i), *(this->reg + i),
                  *(this->reg + i));
        }
    }
}

void Register::reg_debug(uint8_t reg_n) const {
    Debug("%-10s\t0x%-10x\t0x%x", *(regs + reg_n), *(this->reg + reg_n),
          *(this->reg + reg_n));
}
