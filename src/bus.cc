#include "bus.h"
#include <cstdint>
#include "mem.h"

Bus::Bus() : mem(new Mem) {}

void Bus::init(const char *filename) { mem->load_bin(filename); }

// Todo: More device
// Now: Mem only
uint64_t Bus::read(const uint64_t addr, const uint8_t size,
                   uint64_t *values) const {
    return this->mem->read(addr, size, values);
}

uint64_t Bus::write(const uint64_t addr, const uint8_t size, uint64_t values) {
    return this->mem->write(addr, size, values);
}

Bus::~Bus() { delete mem; }
