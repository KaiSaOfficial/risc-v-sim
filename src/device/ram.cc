#include "./device/device.h"
#include "./device/ram.h"

#include "debug.h"
#include <cstring>

RAM::RAM(void) : PRT_RAM(new uint8_t[this->RAM_SIZE]) {
    assert(PRT_RAM != nullptr);

    std::memset((void *)PRT_RAM, 0x00, this->RAM_SIZE);

    Log("device name: %s", "RAM");
    Log("physical memory area [%p, %p]", PRT_RAM, PRT_RAM + this->RAM_SIZE);
};

uint64_t RAM::read(const uint64_t addr, const uint8_t size,
                   uint64_t *values) const {
    uint64_t value = 0x0000;

    if (size != 8 && size != 16 && size != 32 && size != 64) {
        *values = value;
        return LoadAccessFault;
    }

    uint8_t nbytes = size / 8;
    uint64_t index = addr - this->RAM_BASE;

    for (uint8_t i = 0; i < nbytes; i++) {
        value |= static_cast<uint64_t>(*(this->PRT_RAM + index + i) << (i * 8));
    }

    *values = value;
    return OK;
};

uint64_t RAM::write(const uint64_t addr, const uint8_t size,
                    const uint64_t values) {
    uint64_t value = values;

    if (size != 8 && size != 16 && size != 32 && size != 64) {
        return StoreAMOAccessFault;
    }

    uint8_t nbytes = size / 8;
    uint64_t index = addr - this->RAM_BASE;

    for (uint8_t i = 0; i < nbytes; i++) {
        uint8_t offset = 8 * i;
        *(this->PRT_RAM + index + i) = ((value >> offset) & 0xFF);
    }

    return OK;
};

RAM::~RAM() { delete[] PRT_RAM; }
