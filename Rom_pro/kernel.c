#include "./inc/freestanding.h"
#include "./inc/printf.h"

static int sum = 0;

void start_kernel(void) {
    /* User code begin */

    printf("Hello World!\n");

    uint8_t i = 0;
    for (i = 0; i < 100; i++) {
        sum = sum + i * i;
    }

    printf("%d \n", sum);
    asm volatile("ebreak");

    /* User code end */
    while (1) {
    }; // stop here!
}
