#ifndef __RAM_H__
#define __RAM_H__

#include "device.h"

// ROM Construction
class RAM {
private:
    uint8_t *const PRT_RAM = nullptr;              // Pointer of RAM
    constexpr static uint64_t RAM_SIZE = RAM_Size; // DRAM_SIZE of ROM
    constexpr static uint64_t RAM_BASE = RAM_Base;
    constexpr static uint64_t RAM_END = RAM_SIZE + RAM_BASE - 1;

public:
    RAM(void);

    uint64_t read(const uint64_t addr, const uint8_t size,
                  uint64_t *values) const;

    uint64_t write(const uint64_t addr, const uint8_t size,
                   const uint64_t values);
    ~RAM();
};

#endif //__RAM_H__
