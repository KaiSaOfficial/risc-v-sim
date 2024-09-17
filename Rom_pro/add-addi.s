    .data
result: .int                        # sum

    .text
	.file	"test.c"
	.globl	main                    # -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   # @main
# %bb.0:
	addi	sp, sp, -32
	sw	ra, 28(sp)
	sw	s0, 24(sp)
	addi	s0, sp, 32
	mv	a2, zero
	sw	a2, -12(s0)
	sw	a0, -16(s0)
	sw	a1, -20(s0)
	sw	a2, -28(s0)
	sw	a2, -24(s0)
	j	.LBB0_1
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	lw	a0, -24(s0)
	addi	a1, zero, 100
	blt	a1, a0, .LBB0_4
	j	.LBB0_2
.LBB0_2:                                #   in Loop: Header=BB0_1 Depth=1
	lw	a0, -24(s0)
	mul	a0, a0, a0
	lw	a1, -28(s0)
	add	a0, a1, a0
	sw	a0, -28(s0)
	j	.LBB0_3
.LBB0_3:                                #   in Loop: Header=BB0_1 Depth=1
	lw	a0, -24(s0)
	addi	a0, a0, 1
	sw	a0, -24(s0)
	j	.LBB0_1
.LBB0_4:
	mv	a0, zero
    la  t0, result     # ld addr to t0
    lw  t1, -28(s0)    # ld result to t1
    sw  t1, 0(t0)      # wr t2 to result
	lw	s0, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32

	ebreak
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.ident	"clang version 10.0.0-4ubuntu1 "
	.section	".note.GNU-stack","",@progbits
	.addrsig
