#include <cstdint>
#include "debug.h"

#include "register.h"

// RISCV 32IM Reg Name
const char *regs[] = {"$0", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                      "s0", "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                      "a6", "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                      "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

void reg_display(const uint32_t *reg_ptr) {
    for (uint8_t i = 0; i < 32; i++) {
        if ((i == 1) || (i == 2) || (i == 3) || (i == 8) || (i == 9)) {
            Debug("%-10s\t0x%-10x\t0x%x", *(regs + i), *(reg_ptr + i),
                  *(reg_ptr + i));
        } else {
            Debug("%-10s\t0x%-10x\t%u", *(regs + i), *(reg_ptr + i),
                  *(reg_ptr + i));
        }
    }
}
