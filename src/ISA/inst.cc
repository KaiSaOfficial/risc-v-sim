#include "inst.h"
#include <cstdint>
#include "cpu.h"
#include "utils.h"

#define INSTPAT_START(name) \
    {                       \
        const void **__instpat_end = &&concat(__instpat_end_, name);
#define INSTPAT_END(name)           \
    concat(__instpat_end_, name) :; \
    }

__attribute__((always_inline)) static inline void pattern_decode(
    const char *str, int len, uint64_t *key, uint64_t *mask, uint64_t *shift) {
    uint64_t __key = 0, __mask = 0, __shift = 0;
#define macro(i)                                         \
    if ((i) >= len)                                      \
        goto finish;                                     \
    else {                                               \
        char c = str[i];                                 \
        if (c != ' ') {                                  \
            __key = (__key << 1) | (c == '1' ? 1 : 0);   \
            __mask = (__mask << 1) | (c == '?' ? 0 : 1); \
            __shift = (c == '?' ? __shift + 1 : 0);      \
        }                                                \
    }

#define macro2(i) \
    macro(i);     \
    macro((i) + 1)
#define macro4(i) \
    macro2(i);    \
    macro2((i) + 2)
#define macro8(i) \
    macro4(i);    \
    macro4((i) + 4)
#define macro16(i) \
    macro8(i);     \
    macro8((i) + 8)
#define macro32(i) \
    macro16(i);    \
    macro16((i) + 16)
#define macro64(i) \
    macro32(i);    \
    macro32((i) + 32)
    macro64(0);
#undef macro
finish:
    *key = __key >> __shift;
    *mask = __mask >> __shift;
    *shift = __shift;
}

__attribute__((always_inline)) static inline void pattern_decode_hex(
    const char *str, int len, uint64_t *key, uint64_t *mask, uint64_t *shift) {
    uint64_t __key = 0, __mask = 0, __shift = 0;
#define macro(i)                                                              \
    if ((i) >= len)                                                           \
        goto finish;                                                          \
    else {                                                                    \
        char c = str[i];                                                      \
        if (c != ' ') {                                                       \
            __key = (__key << 4) | (c == '?'                 ? 0              \
                                    : (c >= '0' && c <= '9') ? c - '0'        \
                                                             : c - 'a' + 10); \
            __mask = (__mask << 4) | (c == '?' ? 0 : 0xf);                    \
            __shift = (c == '?' ? __shift + 4 : 0);                           \
        }                                                                     \
    }

    macro16(0);
#undef macro
finish:
    *key = __key >> __shift;
    *mask = __mask >> __shift;
    *shift = __shift;
}

// --- pattern matching wrappers for decode ---
#define INSTPAT(pattern, ...)                                          \
    do {                                                               \
        uint64_t key, mask, shift;                                     \
        pattern_decode(pattern, STRLEN(pattern), &key, &mask, &shift); \
        if ((((uint64_t)INSTPAT_INST(s) >> shift) & mask) == key) {    \
            INSTPAT_MATCH(s, ##__VA_ARGS__);                           \
            goto *(__instpat_end);                                     \
        }                                                              \
    } while (0)

#define INSTPAT_INST(s) ((s)->isa.inst.val)
#define INSTPAT_MATCH(s, name, type, ... /* execute body */) {
static int decode_exec(Cpu *cpu) {
    int rd = 0;
    uint32_t src1 = 0, src2 = 0, imm = 0;

#define INSTPAT_MATCH(s, name, type, ... /* execute body */)                  \
    {                                                                         \
        cpu->decode_operand(s, &rd, &src1, &src2, &imm, concat(TYPE_, type)); \
        __VA_ARGS__;                                                          \
    }

    INSTPAT_START();
    INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc, U,
            R(rd) = s->pc + imm);
    INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu, I,
            R(rd) = Mr(src1 + imm, 1));
    INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb, S,
            Mw(src1 + imm, 1, src2));

    INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak, N,
            NEMUTRAP(s->pc, R(10))); // R(10) is $a0
    INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv, N, INV(s->pc));
    INSTPAT_END();

    R(0) = 0; // reset $zero to 0

    return 0;
}
}