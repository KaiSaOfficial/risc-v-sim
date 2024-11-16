#include "./inc/freestanding.h"

int sum = 0;

void start_kernel(void) {
    uint8_t i = 0;
    for (i = 0; i < 100; i++) {
        sum = sum + i * i;
    }

    asm volatile("ebreak");
    while (1) {
    }; // stop here!
}
