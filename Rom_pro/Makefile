# Build path
BUILD_DIR = build
TARGET = asm_pro

# Asm sources
ASM_SOURCES = \
./add-addi.s

C_SOURCES = \
./kernel.c

# binaries
#######################################
AS = clang
BIN = llvm-objcopy

CFLAGS = -Wl,-Ttext=0x0 -nostdlib -mno-relax --target=riscv64 -march=rv32i

ASM_OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

C_OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.s $(sort $(dir $(C_SOURCES)))

all: Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $(ASM_SOURCES) -o $(ASM_OBJECTS)
	$(AS) -c $(CFLAGS) $(C_SOURCES) -o $(C_OBJECTS)
	$(BIN) -O binary $(OBJECTS) $(C_OBJECTS) $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)

