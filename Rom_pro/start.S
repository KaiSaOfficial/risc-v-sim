#include "inc/platform.h"

        # size of stack is 256 bytes
        .equ    STACK_SIZE, 256
        .global _start
        .text
_start:
        la      sp, MEM + STACK_SIZE     # set the initial stack pointer
        j       start_kernel             # jump to kernel
        .end                             # end of file
