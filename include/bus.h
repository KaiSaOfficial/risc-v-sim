#ifndef __BUS_H__
#define __BUS_H__

#include "mem.h"

class Bus {
private:
    Mem *mem;

public:
    Bus();

    void init(const char *filename);

    uint64_t read(const uint64_t addr, const uint8_t size,
                  uint64_t *values) const;

    uint64_t write(const uint64_t addr, const uint8_t size,
                   const uint64_t values);

    ~Bus();
};

#endif //__BUS_H__