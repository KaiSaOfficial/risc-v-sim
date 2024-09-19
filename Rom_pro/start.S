
#include "inc/platform.h"

        # size of stack is 256 bytes
        .equ    STACK_SIZE, 256
        .global _start
        .text
_start:
        la      sp, RAM + STACK_SIZE     # set the initial stack pointer to 0x00001100 (0x00001000 + 256)
        j       start_kernel             # jump to kernel
        .end                             # end of file
