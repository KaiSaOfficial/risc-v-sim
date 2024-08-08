#ifndef __MEM_H__
#define __MEM_H__

#include "utils.h"

#define OK 0
#define LoadAccessFault -1
#define StoreAMOAccessFault -2
#define IllegalInstruction -3

// Mem Construction
class Mem {
private:
    uint8_t *PRT_MEM = nullptr;                              // ptr of Mem
    constexpr static uint64_t DRAM_SIZE = 1024 * 1024 * 128; // DRAM_SIZE of Mem
    const uint64_t DRAM_BASE = 0x8000'0000;
    const uint64_t DRAM_END = DRAM_SIZE + DRAM_BASE - 1;

public:
    Mem(void);
    uint64_t load_bin(const char *);

    uint64_t read(const uint64_t addr, const uint8_t size,
                  uint64_t *values) const;

    uint64_t write(const uint64_t addr, const uint8_t size,
                   const uint64_t values);

    void debug(uint16_t addr) const;

    ~Mem();
};

#endif //__MEM_H__
