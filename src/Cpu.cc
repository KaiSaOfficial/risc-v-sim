#include "Cpu.h"
#include <cstdint>
#include <cstdio>
#include "Mem.h"
#include "Register.h"

uint32_t read_instruction(const Mem *mem, uint32_t *pc) {
    uint32_t op = 0x00000000;
    op |= mem->read(*pc) << 0;
    op |= mem->read(*pc) << 8;
    op |= mem->read(*pc) << 16;
    op |= mem->read(*pc) << 24;

    *pc = *pc + 4;
    return op;
}

void run(void) {
    Register zero_reg(zero);
    Register rs1_reg(rs1);
    Register rs2_reg(rs2);
    Register rd_reg(rd);

    uint32_t pc = 0x20;
    Mem mem;

    while (1) {
        uint32_t instruction = read_instruction(&mem, &pc);
        uint16_t opcode = instruction & 0x7F;

        switch (opcode) {
            case 0x33: {
                std::printf("add!\n");
                rs1_reg.write((instruction >> 15) & 0x1F);
                rs2_reg.write((instruction >> 20) & 0x1F);
                rd_reg.write((instruction >> 7) & 0x1F);

                uint32_t funct7 = (instruction >> 25) & 0x7F;

                switch (funct7) {
                    case 0x00: {
                        uint32_t res = rs1_reg.read() + rs2_reg.read();
                        rd_reg.write(res);
                    };

                    case 0x20: {
                        uint32_t res = rs1_reg.read() - rs2_reg.read();
                        rd_reg.write(res);
                    };

                    default: std::printf("Unknown support funct7:%x", funct7);
                }
            }
            case 0x13: {
                std::printf("addi!\n");
                rs1_reg.write((instruction >> 15) & 0x1F);
                rd_reg.write((instruction >> 7) & 0x1F);
                uint32_t imm =
                    ((instruction >> 20) & 0xFFF) | ((instruction >> 31) << 11);
                uint32_t result = rs1_reg.read() + imm;
                rd_reg.write(result);
            }
        }
    }

    return;
}
