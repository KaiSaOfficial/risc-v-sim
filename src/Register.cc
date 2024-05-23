#include "cassert"
#include "cstdint"
#include "cstdio"
#include "cstring"
#include <cassert>
#include <cstdint>

#include "Register.h"

Register::Register(Reg_T type_) : reg(0x0000), type(type_){};

uint32_t Register::read(void) const { return reg; }

void Register::write(uint32_t Value) { this->reg = Value; }

void Register::debug(uint16_t Address) const { std::printf("%x", this->reg); }
