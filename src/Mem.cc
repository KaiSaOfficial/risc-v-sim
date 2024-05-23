#include "cassert"
#include "cstdint"
#include "cstdio"
#include "cstring"
#include <cassert>
#include <cstdint>

#include "Mem.h"

Mem::Mem(void) {
    ptr = new uint8_t[2048];
    if (ptr == nullptr) {
        throw "No Mem Allocated";
    }

    std::memset((void *)ptr, 0x00, 2048);
};

uint8_t Mem::read(uint16_t Address) const {
    assert(Address < 2048);
    return *(this->ptr + Address);
};

void Mem::write(uint16_t Address, uint8_t Value) {
    *(this->ptr + Address) = Value;
};

void Mem::debug(uint16_t Address) const {
    std::printf("%x", *(this->ptr + Address));
};

Mem::~Mem() { delete ptr; }

// test of Mem
// PASS!!!!!
/*
int main(int arg, const char *argv[]) {
    Mem mem;

    for (uint16_t i = 0; i < 2048; i++) {
        std::printf("%d: \t value: %d\n", i, mem.read(i));
    }

    for (uint16_t i = 0; i < 2048; i++) {
        mem.write(i, i);
    }

    for (uint16_t i = 0; i < 2048; i++) {
        std::printf("%d: \t value: %d\n", i, mem.read(i));
    }

    return 0;
}
*/
