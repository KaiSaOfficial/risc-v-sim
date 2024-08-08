#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "bus.h"
#include "register.h"

uint32_t read_instruction(const Bus *bus, const uint64_t pc) {
    uint64_t op = 0x00000000;
    bus->read(pc, 32, &op);
    uint32_t ins = op & 0xFFFFFFFF;

    return ins;
}

int main(int argc, const char *argv[]) {
    Register reg;
    Bus bus;

    uint64_t pc = 0x8000'0000;

    if (argc < 3) {
        std::cerr << "Error: No bin file." << std::endl;
        std::cerr << "Usage: program_name - <filename>.bin" << std::endl;
        return 1; // 返回错误状态码
    }

    bus.init(argv[2]);

    while (1) {
        uint32_t instruction = read_instruction(&bus, pc);
        uint8_t opcode = instruction & 0x7F;

        switch (opcode) {
            case 0x33: {
                uint8_t rd = (instruction >> 7) & 0x1F;
                uint8_t rs1 = (instruction >> 15) & 0x1F;
                uint8_t rs2 = (instruction >> 20) & 0x1F;

                uint8_t funct3 = (instruction >> 12) & 0x7;
                uint8_t funct7 = (instruction >> 25) & 0x7F;

                switch (funct7) {
                    case 0x00: {
                        std::printf("add!\n");
                        uint32_t result = reg.read(rs1) + reg.read(rs2);
                        reg.write(result, rd);

                        break;
                    };

                    case 0x20: {
                        std::printf("sub!\n");
                        uint32_t result = reg.read(rs1) - reg.read(rs2);
                        reg.write(result, rd);

                        break;
                    };

                    default: std::printf("Unknown support funct7:%x", funct7);
                }

                pc = pc + 4;

                break;
            };

            case 0x13: {
                std::printf("addi!\n");
                uint8_t rd = (instruction >> 7) & 0x1F;
                uint8_t rs1 = (instruction >> 15) & 0x1F;
                uint32_t imm = ((instruction >> 20) & 0xFFF);

                uint32_t result = reg.read(rs1) + imm;
                reg.write(result, rd);

                pc = pc + 4;

                break;
            }

            default: {
                reg.debug(30);
                std::printf("Unknow ins\n");
                return 1;
            }
        }
    }

    return 0;
}
