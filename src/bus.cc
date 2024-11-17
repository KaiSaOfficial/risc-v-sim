#include "bus.h"
#include "debug.h"

Bus::Bus() : rom(new ROM()), ram(new RAM()), uart(new UART()) { Log("Bus"); }

void Bus::init(const char *filename) { rom->load_bin(filename); }

// Now: ROM, RAM, UART only
uint64_t Bus::read(const uint64_t addr, const uint8_t size,
                   uint64_t *values) const {
    if (FLASH_Base <= addr && addr < FLASH_Base + FLASH_Size - 1) {
        return this->rom->read(addr, size, values);
    } else if (RAM_Base <= addr && addr < RAM_Base + RAM_Size - 1) {
        return this->ram->read(addr, size, values);
    } else if (UART_Base <= addr && addr < UART_Base + UART_Size - 1) {
        return this->uart->read(addr, size, values);
    } else {
        return LoadAccessFault;
    }
}

uint64_t Bus::write(const uint64_t addr, const uint8_t size, uint64_t values) {
    if (FLASH_Base < addr && addr < FLASH_Base + FLASH_Size) {
        return this->rom->write(addr, size, values);
    } else if (RAM_Base < addr && addr < RAM_Base + RAM_Size) {
        return this->ram->write(addr, size, values);
    } else if (UART_Base < addr && addr < UART_Base + UART_Size) {
        return this->uart->write(addr, size, values);
    } else {
        return LoadAccessFault;
    }
}

Bus::~Bus() {
    delete rom;
    delete ram;
    delete uart;
}
