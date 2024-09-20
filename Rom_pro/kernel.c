#include "./inc/freestanding.h"

void start_kernel(void) {
    uint16_t sum = 0;
    uint8_t i = 0;
    for (i = 0; i < 100; i++) {
        sum = sum + i * i;
    }

    asm volatile("ebreak");

    while (1) {
    }; // stop here!
}