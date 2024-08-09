#include <cstdint>
#include <iostream>

#include "utils.h"
#include "cpu.h"
#include "register.h"
#include "decoder.h"

Cpu::Cpu(void) {
    this->bus = new Bus;
    this->reg = new Register;
    this->pc = 0x8000'0000;
    this->state = CPU_STOP;
}

void Cpu::Cpu_init(const char *filename) {
    // load Bin
    this->bus->init(filename);

    // $0: 0000'0000
    this->reg->write(0x0000'0000, 0);
}

void Cpu::ifetch(uint32_t *ins) {
    uint64_t op = 0x0000'0000'0000'0000;
    bus->read(this->pc, 32, &op);

    *ins = op & 0xffff'ffff;
}

void Cpu::decode_operand(uint32_t instruction, uint32_t *rd, uint32_t *src1,
                         uint32_t *src2, uint32_t *imm, int32_t type) {
    uint8_t rs1 = (instruction >> 15) & 0x1f;
    uint8_t rs2 = (instruction >> 20) & 0x1f;
    *rd = (instruction >> 7) & 0x1f;

    switch (type) {
        case TYPE_I: {
            *src1 = this->reg->read(rs1);
            *imm = (instruction >> 20) & 0xff'f;
            break;
        }
        case TYPE_U: {
            *imm = (instruction >> 12) & 0xf'ffff;
            break;
        }
        case TYPE_S: {
            *src1 = this->reg->read(rs1);
            *src2 = this->reg->read(rs2);
            *imm = ((instruction >> 5) & 0x1f) |
                   (((instruction >> 25) & 0x7f) << 5);
            break;
        }
    }
}

void Cpu::decode_exec(void) {
    uint32_t rd = 0;
    uint32_t src1 = 0, src2 = 0, imm = 0;
}

int main(int argc, const char *argv[]) {
    if (argc < 3) {
        std::cerr << "Error: No bin file." << std::endl;
        std::cerr << "Usage: program_name - <filename>.bin" << std::endl;
        return 1;
    }

    Cpu cpu;
    cpu.Cpu_init(argv[2]);

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
                reg.reg_display();
                std::printf("Unknow ins\n");
                return 1;
            }
        }
    }

    return 0;
}
