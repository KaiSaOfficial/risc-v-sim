# Build path
BUILD_DIR = build
TARGET = asm_pro

# Asm sources
ASM_SOURCES = \
./add-addi.s

# binaries
#######################################
AS = clang
BIN = llvm-objcopy

CFLAGS = -Wl,-Ttext=0x0 -nostdlib -mno-relax --target=riscv64 -march=rv64g

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

all: Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $(ASM_SOURCES) -o $(OBJECTS)
	$(BIN) -O binary $(OBJECTS) $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR $(BUILD_DIR)

