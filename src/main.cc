#include <cstdio>
#include "cpu.h"
#include "utils.h"
#include "debug.h"

int main(int argc, const char *argv[]) {
    /*
    if (argc < 2) {
        Error("No correct input");
        Error("emu program.bin");
        return 1;
    }
    */

    const char *str = "/home/subingdong/risc-v-sim/Rom_pro/build/asm_pro.bin";

    Cpu cpu;
    cpu.cpu_init(str);

    uint32_t ins;

    while (cpu.state.state == CPU_RUNNING) {
        cpu.ifetch(&ins);
        cpu.decode_exec(ins);
    }

    cpu.debug_reg();
    cpu.debug_mem(0x80000018);

    return 0;
}
