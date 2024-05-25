#include "cassert"
#include "cstdint"
#include "cstdio"
#include "cstring"
#include <cassert>
#include <cstdint>

#include "mem.h"

Mem::Mem(void) {
    PRT_MEM = new uint8_t[this->DRAM_SIZE];
    assert(PRT_MEM != nullptr);

    std::memset((void *)PRT_MEM, 0x00, this->DRAM_SIZE);
};

void Mem::load_pro(const char *filename) {
    FILE *file = nullptr;
    file = fopen(filename, "rb");

    assert(file != nullptr);

    // litte end
    if (fread((void *)(this->PRT_MEM), sizeof(uint8_t), this->DRAM_SIZE,
              file)) {
        std::printf("load prog!\n");
    }

    fclose(file);
}

uint64_t Mem::read(const uint64_t addr, const uint8_t size,
                   uint64_t *values) const {
    uint64_t value = 0x0000;

    if (size != 8 && size != 16 && size != 32 && size != 64) {
        *values = value;
        return LoadAccessFault;
    }

    uint8_t nbytes = size / 8;
    uint64_t index = addr - this->DRAM_BASE;

    for (uint8_t i = 0; i < nbytes; i++) {
        value |= static_cast<uint64_t>(*(this->PRT_MEM + index + i) << (i * 8));
    }

    *values = value;
    return OK;
};

uint64_t Mem::write(const uint64_t addr, const uint8_t size,
                    const uint64_t values) {
    uint64_t value = 0x00;

    if (size != 8 && size != 16 && size != 32 && size != 64) {
        return StoreAMOAccessFault;
    }

    uint8_t nbytes = size / 8;
    uint64_t index = addr - this->DRAM_BASE;

    for (uint8_t i = 0; i < nbytes; i++) {
        uint8_t offset = 8 * i;
        *(this->PRT_MEM + index + i) = ((value >> offset) & 0xFF);
    }

    return OK;
};

void Mem::debug(uint16_t addr) const {
    std::printf("%x\n", *(this->PRT_MEM + addr));
};

Mem::~Mem() { delete[] PRT_MEM; }

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
