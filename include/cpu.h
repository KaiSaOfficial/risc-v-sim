#ifndef __CPU_H__
#define __CPU_H__

#include <cstdint>
#include "utils.h"
#include "bus.h"
#include "register.h"

class Cpu {
private:
    // Bus file
    Bus *const bus;

    // Register Files
    uint32_t *const reg;
    uint32_t pc;

    void decode_operand(uint32_t instruction, uint32_t *rd, uint32_t *src1,
                        uint32_t *src2, uint32_t *imm, int32_t type);

public:
    Cpu();

    // Load program && init Reg and Mem
    void cpu_init(const char *filename);

    // fecth ins from mem
    void ifetch(uint32_t *);

    void decode_exec(const uint32_t ins);

    void debug_reg(void) const;
    // CPU state
    CPU_Info state;
};

#endif //__CPU_H__
