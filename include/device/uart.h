#ifndef __UART_H__
#define __UART_H__

#include "device.h"

// UART Construction
class UART {
private:
    uint8_t *const PRT_UART = nullptr;               // Pointer of RAM
    constexpr static uint64_t UART_SIZE = UART_Size; // DRAM_SIZE of Rom
    constexpr static uint64_t UART_BASE = UART_Base;
    constexpr static uint64_t UART_END = UART_SIZE + UART_BASE - 1;

public:
    UART(void);

    uint64_t read(const uint64_t addr, const uint8_t size,
                  uint64_t *values) const;

    uint64_t write(const uint64_t addr, const uint8_t size,
                   const uint64_t values);
    ~UART();
};

#endif //__UART_H__
