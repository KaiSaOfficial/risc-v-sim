CFLAGS = -Wl,-Ttext=0x0 -nostdlib -mno-relax --target=riscv32 -march=rv32i

CC = clang
OBJDUMP = llvm-objdump

SRCS_ASM = \
	   start.S \

SRCS_C = \
	kernel.c \

OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.c=.o)

.DEFAULT_GOAL := all
all: os.elf

# start.o must be the first in dependency!
os.elf: ${OBJS}
	${CC} ${CFLAGS} -o os.elf  $^
	@${OBJDUMP} -O binary os.elf os.bin

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

clean:
	rm -fr *.o *.bin *.elf

