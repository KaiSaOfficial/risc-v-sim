#include <cstdint>
#include <cstdio>
#include <cstring>

#include "utils.h"
#include "cpu.h"
#include "inst.h"
#include "decoder.h"
#include "debug.h"

void set_nemu_state(Cpu *cpu, CPU_State state, uint32_t pc, uint32_t halt_ret) {
    cpu->state.state = state;
    cpu->state.halt_pc = pc;
    cpu->state.halt_ret = halt_ret;
}

Cpu::Cpu(void) : bus(new Bus), reg(new uint32_t[32]) {
    assert(this->reg != nullptr);

    memset((void *)(this->reg), 0x0000'0000, 32);
    Log("physical reg area [%p, %p]", this->reg, this->reg + 32);

    this->pc = 0x8000'0000;
    this->reg[2] = 0x8000'0400;

    set_nemu_state(this, CPU_STOP, 0x8000'0000, 0x8000'0000);
}

void Cpu::cpu_init(const char *filename) {
    // load Bin
    this->bus->init(filename);

    // $0: 0000'0000
    this->reg[0] = 0x0000'0000;

    set_nemu_state(this, CPU_RUNNING, 0x8000'0000, 0x8000'0000);
}

void Cpu::ifetch(uint32_t *ins) {
    uint64_t op = 0x0000'0000'0000'0000;
    bus->read(this->pc, 32, &op);

    *ins = op & 0xffff'ffff;
}

inline void Cpu::decode_operand(uint32_t instruction, uint32_t *rd,
                                uint32_t *src1, uint32_t *src2, int32_t *imm,
                                int32_t type) {
    uint8_t rs1 = (instruction >> 15) & 0x1f;
    uint8_t rs2 = (instruction >> 20) & 0x1f;
    *rd = (instruction >> 7) & 0x1f;

    switch (type) {
        case TYPE_I: {
            *src1 = this->reg[rs1];
            // I 12-bits
            *imm = ((instruction >> 20) & 0xff'f) & 0xfff;

            if (*imm & 0x0800) {
                *imm |= 0xffff'f000;
            }

            break;
        }
        case TYPE_U: {
            // U 20-bits
            *imm = (((instruction >> 12) & 0xf'ffff) & 0xf'ffff) << 12;

            break;
        }
        case TYPE_S: {
            *src1 = this->reg[rs1];
            *src2 = this->reg[rs2];
            *imm = (((instruction >> 7) & 0x1f) |
                    (((instruction >> 25) & 0x7f) << 5));

            *imm = *imm & 0x0fff;

            if (*imm & 0x0700) {
                *imm |= 0xffff'f000;
            }

            break;
        }
        case TYPE_J: {
            *imm = ((((instruction >> 12) & 0xff) << 11) |
                    (((instruction >> 20) & 0x01) << 10) |
                    ((instruction >> 21) & 0x3'ff) |
                    (((instruction >> 31) & 0x01) << 19))
                   << 1;

            *imm = *imm & 0x1f'ffff;

            if (*imm & 0x10'000) {
                *imm |= 0xffe0'0000;
            }

            break;
        }
        case TYPE_B: {
            *src1 = this->reg[rs1];
            *src2 = this->reg[rs2];
            *imm = (((((instruction >> 7) & 0x03) << 10) |
                     ((instruction >> 8) & 0x0f) |
                     (((instruction >> 25) & 0x3f) << 4) |
                     (((instruction >> 31) & 0x01) << 11))
                    << 1);

            // B 13-bits
            *imm = *imm & 0x1fff;

            if (*imm & 0x1000) {
                *imm |= 0xffff'e000;
            }

            break;
        }
        case TYPE_R: {
            *src1 = this->reg[rs1];
            *src2 = this->reg[rs2];

            break;
        }
    }
}

void Cpu::decode_exec(const uint32_t ins) {
    uint32_t rd = 0;
    uint32_t src1 = 0, src2 = 0;
    int32_t imm = 0;

    static int32_t dnpc = 0;
    int32_t snpc = this->pc + 4;
    dnpc = snpc;

#define INSTPAT_MATCH(ins, name, type, ... /* execute body */) \
    {                                                          \
        this->decode_operand(ins, &rd, &src1, &src2, &imm,     \
                             concat(TYPE_, type));             \
        __VA_ARGS__;                                           \
    }

    INSTPAT_START()

    INSTPAT("??????? ????? ????? 000 ????? 00100 11", li, I,
            R(rd) = src1 + imm);
    INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui, U, R(rd) = imm);
    INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc, U,
            R(rd) = this->pc + imm);
    INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw, I,
            uint64_t tmp = 0x0000'0000;
            Mr(src1 + imm, 32, &tmp);
            R(rd) = (tmp & 0xffff'ffff)); // 从内存相应位置读出并写入到寄存器中
    INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu, I,
            uint64_t tmp = 0x0000'0000;
            Mr(src1 + imm, 8, &tmp); R(rd) = (tmp & 0xffff'ffff));
    INSTPAT("??????? ????? ????? 000 ????? 00000 11", lb, I,
            uint64_t tmp = 0x0000'0000;
            Mr(src1 + imm, 8, &tmp); R(rd) = (tmp & 0x0000'00ff));
    INSTPAT("??????? ????? ????? 001 ????? 00000 11", lh, I,
            uint64_t tmp = 0x0000'0000;
            Mr(src1 + imm, 16, &tmp); R(rd) = (tmp & 0x0000'ffff));
    INSTPAT("??????? ????? ????? 101 ????? 00000 11", lhu, I,
            uint64_t tmp = 0x0000'0000;
            Mr(src1 + imm, 8, &tmp); R(rd) = tmp);
    INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi, I,
            R(rd) = imm & src1);
    INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb, S,
            Mw(src1 + imm, 8, src2));
    INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw, S,
            Mw(src1 + imm, 32, src2)); // 向内存中写入
    INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh, S,
            Mw(src1 + imm, 16, src2));
    INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal, J,
            R(rd) = this->pc + 4;
            dnpc += imm - 4;); // jal指令
    INSTPAT("??????? ????? ????? 100 ????? 00100 11", xori, I,
            R(rd) = src1 ^ imm);
    INSTPAT("??????? ????? ????? 110 ????? 00100 11", ori, I,
            R(rd) = src1 | imm);
    INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi, I,
            R(rd) = src1 + imm); // addi指令
    INSTPAT("0100000 ????? ????? 101 ????? 00100 11", srai, I,
            imm = imm & 0x0000'001f;
            R(rd) = (src1 >> 31) << (32 - imm) | (src1 >> imm));
    INSTPAT("0000000 ????? ????? 101 ????? 00100 11", srli, I,
            R(rd) = src1 >> imm);
    INSTPAT("0000000 ????? ????? 001 ????? 00100 11", elli, I,
            R(rd) = src1 << imm);
    INSTPAT("??????? ????? ????? 000 ????? 11001 11", ret, I,
            R(rd) = this->pc + 4;
            dnpc = (src1 + imm) & ~1); // jalr(ret)指令
    INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt, B,
            dnpc += (int32_t)src1 < (int32_t)src2 ? imm - 4 : 0);
    INSTPAT("??????? ????? ????? 110 ????? 11000 11", bltu, B,
            dnpc += (uint32_t)src1 < (uint32_t)src2 ? imm - 4 : 0);
    INSTPAT("??????? ????? ????? 101 ????? 11000 11", bge, B,
            dnpc += (int32_t)src1 >= (int32_t)src2 ? imm - 4 : 0);
    INSTPAT("??????? ????? ????? 111 ????? 11000 11", bgeu, B,
            dnpc += src1 >= src2 ? imm - 4 : 0;);
    INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq, B,
            dnpc += src1 == src2 ? imm - 4 : 0;);
    INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne, B,
            dnpc += src1 != src2 ? imm - 4 : 0;);
    INSTPAT("0100000 ????? ????? 101 ????? 01100 11", sra, R,
            R(rd) = (src1 >> 31) << (32 - imm) | (src1 >> src2));
    INSTPAT("0000000 ????? ????? 101 ????? 01100 11", srl, R,
            R(rd) = src1 >> src2);
    INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem, R,
            R(rd) = (int32_t)src1 % (int32_t)src2);
    INSTPAT("0000001 ????? ????? 111 ????? 01100 11", remu, R,
            R(rd) = src1 % src2);
    INSTPAT("0000001 ????? ????? 101 ????? 01100 11", divu, R,
            R(rd) = (uint32_t)src1 / (uint32_t)src2);
    INSTPAT("0000001 ????? ????? 100 ????? 01100 11", div, R,
            R(rd) = (int32_t)src1 / (int32_t)src2);
    INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul, R,
            R(rd) = src1 * src2);
    INSTPAT("0000001 ????? ????? 001 ????? 01100 11", mulh, R, int32_t a = src1;
            int32_t b = src2; int64_t tmp = (int64_t)a * (int64_t)b;
            R(rd) = (tmp >> 32) & 0xffff'ffff);
    INSTPAT("0000001 ????? ????? 011 ????? 01100 11", mulhu, R,
            uint64_t tmp = (uint64_t)src1 * (uint64_t)src2;
            R(rd) = (tmp >> 32) & 0xffff'ffff);
    INSTPAT("0000000 ????? ????? 111 ????? 01100 11", and, R,
            R(rd) = src1 & src2);
    INSTPAT("0000000 ????? ????? 001 ????? 01100 11", sll, R,
            R(rd) = src1 << src2);
    INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add, R,
            R(rd) = src1 + src2);
    INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub, R,
            R(rd) = src1 - src2);
    INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu, R,
            R(rd) = (uint32_t)src1 < (uint32_t)src2 ? 1 : 0;);
    INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltiu, I,
            R(rd) = (uint32_t)src1 < (uint32_t)imm ? 1 : 0);
    INSTPAT("??????? ????? ????? 010 ????? 00100 11", slti, I,
            R(rd) = (int32_t)src1 < (int32_t)imm ? 1 : 0);
    INSTPAT("0000000 ????? ????? 010 ????? 01100 11", slt, R,
            R(rd) = (int32_t)src1 < (int32_t)src2 ? 1 : 0);
    INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor, R,
            R(rd) = src1 ^ src2);
    INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or, R,
            R(rd) = src1 | src2);
    INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak, N,
            set_nemu_state(this, CPU_STOP, this->pc, R(10))); // R(10) is $a0

    INSTPAT_END()
    /*

    INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv, N,
            INV(this->pc)); // 对所有模式都无法匹配的指令，判定为非法指令
        */

    R(0) = 0; // reset $zero to 0
    this->pc = dnpc;
};

void Cpu::debug_reg(void) const { reg_display(this->reg); }

void Cpu::debug_mem(const uint64_t addr) const {
    uint64_t value = 0;

    this->bus->read(addr, 32, &value);
    Debug("Mem: \t0x%-10lx\t%lu", addr, value);
}
