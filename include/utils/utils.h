#ifndef __UTLIS_H__
#define __UTLIS_H__

#include <cstdio>
#include <cstdint>

typedef enum { CPU_RUNNING, CPU_STOP, CPU_END, CPU_ABORT, CPU_QUIT } CPU_State;

// ------------- log color -------------
#define ANSI_FG_BLACK "\33[1;30m"
#define ANSI_FG_RED "\33[1;31m"
#define ANSI_FG_GREEN "\33[1;32m"
#define ANSI_FG_YELLOW "\33[1;33m"
#define ANSI_FG_BLUE "\33[1;34m"
#define ANSI_FG_MAGENTA "\33[1;35m"
#define ANSI_FG_CYAN "\33[1;36m"
#define ANSI_FG_WHITE "\33[1;37m"
#define ANSI_BG_BLACK "\33[1;40m"
#define ANSI_BG_RED "\33[1;41m"
#define ANSI_BG_GREEN "\33[1;42m"
#define ANSI_BG_YELLOW "\33[1;43m"
#define ANSI_BG_BLUE "\33[1;44m"
#define ANSI_BG_MAGENTA "\33[1;35m"
#define ANSI_BG_CYAN "\33[1;46m"
#define ANSI_BG_WHITE "\33[1;47m"
#define ANSI_NONE "\33[0m"

#define str(x) str_temp(x)
// strlen() for string constant
#define STRLEN(CONST_STR) (sizeof(CONST_STR) - 1)

#define ANSI_FMT(str, fmt) fmt str ANSI_NONE

#define _Log(...)            \
    do {                     \
        printf(__VA_ARGS__); \
    } while (0)

#endif //__UTLIS_H__
