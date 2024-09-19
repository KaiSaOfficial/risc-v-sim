#include "cpu.h"
#include "utils.h"
#include "debug.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        Error("No correct input");
        Error("emu program.bin");
        return 1;
    }

    // const char *str = "/home/subingdong/risc-v-sim/Rom_pro/build/os.bin";

    Cpu cpu;
    cpu.cpu_init(argv[1]);

    uint32_t ins;

    while (cpu.state.state == CPU_RUNNING) {
        cpu.ifetch(&ins);
        cpu.decode_exec(ins);
    }

    cpu.debug_reg();

    return 0;
}
