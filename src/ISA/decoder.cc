#include "decoder.h"
#include "utils.h"

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