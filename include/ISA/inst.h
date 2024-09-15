#ifndef __INST_H__
#define __INST_H__

#include <cstdint>
#include "utils.h"

#define R(i) (*(this->reg + i))
#define Mr (this->bus->read)
#define Mw (this->bus->write)

// --- pattern matching mechanism ---
#define INSTPAT_START(name)          \
    {                                \
        const void **__instpat_end = \
            (const void **)&&concat(__instpat_end_, name);
#define INSTPAT_END(name)           \
    concat(__instpat_end_, name) :; \
    }

__attribute__((always_inline)) static inline void pattern_decode(
    const char *str, int len, uint32_t *key, uint32_t *mask, uint32_t *shift) {
    uint32_t __key = 0, __mask = 0, __shift = 0;
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

// --- pattern matching wrappers for decode ---
#define INSTPAT(pattern, ...)                                          \
    do {                                                               \
        uint32_t key, mask, shift;                                     \
        pattern_decode(pattern, STRLEN(pattern), &key, &mask, &shift); \
        if ((((uint32_t)(ins) >> shift) & mask) == key) {              \
            INSTPAT_MATCH(ins, ##__VA_ARGS__);                         \
            goto *(__instpat_end);                                     \
        }                                                              \
    } while (0)

#endif //__INST_H__
