#include "./device/device.h"
#include "./device/uart.h"

#include "debug.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

UART::UART(void) : PRT_UART(new uint8_t[this->UART_SIZE]) {
    assert(PRT_UART != nullptr);

    std::memset((void *)PRT_UART, 0x00, this->UART_SIZE);

    Log("device name: %s", "UART");
    Log("physical memory area [%p, %p]", PRT_UART, PRT_UART + this->UART_SIZE);
};

uint64_t UART::read(const uint64_t addr, const uint8_t size,
                    uint64_t *values) const {
    uint64_t value = 0x0000;

    if (size != 8 && size != 16 && size != 32 && size != 64) {
        *values = value;
        return LoadAccessFault;
    }

    uint8_t nbytes = size / 8;
    uint64_t index = addr - this->UART_BASE;

    for (uint8_t i = 0; i < nbytes; i++) {
        value |=
            static_cast<uint64_t>(*(this->PRT_UART + index + i) << (i * 8));
    }

    *values = value;
    return OK;
};

uint64_t UART::write(const uint64_t addr, const uint8_t size,
                     const uint64_t values) {
    uint64_t value = values;

    if (size != 8 && size != 16 && size != 32 && size != 64) {
        return StoreAMOAccessFault;
    }

    uint8_t nbytes = size / 8;
    uint64_t index = addr - this->UART_BASE;

    for (uint8_t i = 0; i < nbytes; i++) {
        uint8_t offset = 8 * i;
        char val = (value >> offset) & 0xFF;
        *(this->PRT_UART + index + i) = val;

        putchar(val);
    }

    return OK;
};

UART::~UART() { delete[] PRT_UART; }
