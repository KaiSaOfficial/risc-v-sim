#ifndef __BUS_H__
#define __BUS_H__

#include "./device/device.h"
#include "./device/mem.h"
#include "./device/ram.h"
#include "./device/uart.h"

class Bus {
private:
    ROM *const rom;
    RAM *const ram;
    UART *const uart;

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