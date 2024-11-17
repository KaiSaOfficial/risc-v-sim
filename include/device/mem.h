#ifndef __ROM_H__
#define __ROM_H__

#include "device.h"

// ROM Construction
class ROM {
private:
    uint8_t *const PRT_MEM = nullptr;                 // ptr of ROM
    constexpr static uint64_t DRAM_SIZE = FLASH_Size; // DRAM_SIZE of ROM
    constexpr static uint64_t DRAM_BASE = FLASH_Base;
    constexpr static uint64_t DRAM_END = DRAM_SIZE + DRAM_BASE - 1;

public:
    ROM(void);
    uint64_t load_bin(const char *);

    uint64_t read(const uint64_t addr, const uint8_t size,
                  uint64_t *values) const;
    uint64_t write(const uint64_t addr, const uint8_t size,
                   const uint64_t values);
    ~ROM();
};

#endif //__ROM_H__
