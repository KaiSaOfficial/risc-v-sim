#include "cpu.h"
#include "utils.h"
#include "debug.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        Error("No correct input, Please type: RISC_EMU program.bin");
        return 1;
    }

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
