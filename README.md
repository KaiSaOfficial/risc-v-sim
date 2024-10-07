# risc-v-sim

This repo is a simple and friendly project of RISC-V Emulator.  It can run compiled machine code and analyze how the CPU works. In some sense, it can be viewed as microcontroller with no OS(Operating System).

For now, We finished the RISCV-IM ISA implementation and an easy BootLoader. Here is an example:

## Machine Code Part
```C
void start_kernel(void) {
    uint16_t sum = 0;
    uint8_t i = 0;
    for (i = 0; i < 100; i++) {
        sum = sum + i * i;
    }

    while (1) {
    }; // stop here!
}
```
The program calculates the total of squares from 1 to 100. You may feel confused because there is no main function. However, the ASM program resets the SP pointer and redirects the PC to the start_kernel Function. Here is the reference link for this part: https://gitee.com/lizhanpeng2022/riscv_os.

```ASM
#include "inc/platform.h"
        # size of stack is 256 bytes
        .equ    STACK_SIZE, 256
        .global _start
        .text
_start:
        la      sp, MEM + STACK_SIZE     # set the initial stack pointer
        j       start_kernel             # jump to kernel
        .end                             # end of file
```
## Emulator
After compiling, the emulator loads the bin file into its own memory and starts to fetch instructions, execute instructions and update the PC. When cpu fetches $ebreak$ instruction, it will exit the loop. Usually, an embedded program contains a while(1)-loop, so if you want to quit, you can add the $ebreak$ ins to force it to quit. In this part, some codes are from nemu prj, Here is the reference of nemu: https://github.com/NJU-ProjectN/nemu

```c
while (cpu.state.state == CPU_RUNNING) {
    cpu.ifetch(&ins);
    cpu.decode_exec(ins);
}
```
## How to build
This project is managed and compiled using xmake, so xmake, g++, and other tools need to be installed. To compile the machine code, a riscv64-unknown-elf-gcc toolchain is also required.

## Todo!
We want to support interrupt and optimize the BootLoader. After that, we may implement more peripherals like GPIO, Serial, SPI and IIC.

