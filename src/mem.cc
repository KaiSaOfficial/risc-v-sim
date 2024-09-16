#include <bits/stdint-uintn.h>
#include <cstring>
#include <cassert>
#include <cstdint>

#include "debug.h"
#include "mem.h"

Mem::Mem(void) : PRT_MEM(new uint8_t[this->DRAM_SIZE]) {
    assert(PRT_MEM != nullptr);

    std::memset((void *)PRT_MEM, 0x00, this->DRAM_SIZE);
    Log("physical memory area [%p, %p]", PRT_MEM, PRT_MEM + this->DRAM_SIZE);
};

uint64_t Mem::load_bin(const char *filename) {
    FILE *file = nullptr;
    file = fopen(filename, "rb");
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    Log("The image is %s, size = %ld", filename, size);

    fseek(file, 0, SEEK_SET);
    int ret = fread((void *)(this->PRT_MEM), sizeof(uint8_t), size, file);

    assert(ret == size);
    fclose(file);

    return size;
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
    uint64_t value = values;

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

Mem::~Mem() { delete[] PRT_MEM; }
