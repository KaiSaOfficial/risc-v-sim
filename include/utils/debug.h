#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "utils.h"

#define Error(format, ...)                                                     \
    _Log(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_RED) "\n", __FILE__, __LINE__, \
         __func__, ##__VA_ARGS__)

#define Log(format, ...)                                               \
    _Log(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_GREEN) "\n", __FILE__, \
         __LINE__, __func__, ##__VA_ARGS__)

#define Debug(format, ...) \
    _Log(ANSI_FMT("[debuging] " format, ANSI_FG_RED) "\n", ##__VA_ARGS__)

#endif //__DEBUG_H__