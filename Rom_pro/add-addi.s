.data
    result: .int      # sum

.text
    .globl _start     # main

_start:
    li t0, 1          # t0 -> 1
    li t1, 100        # t1 -> 100
    li t2, 0          # t2 -> 0

sum_loop:
    add t2, t2, t0    # t2 = t2 + t0
    addi t0, t0, 1    # t0 = t0 + 1
    blt t0, t1, sum_loop # t0 < t1, jump loop

    la a0, result     # ld ra to a0
    sw t2, 0(a0)      # wr t2 to result

    # quit
    ebreak            # software break point