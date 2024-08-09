#ifndef __REGISTER_H__
#define __REGISTER_H__

#include <cstdint>
#include "cstdint"

class Register {
private:
    uint32_t *reg = nullptr;
    const uint16_t REG_N = 32;

public:
    Register();
    ~Register();
    uint32_t read(uint8_t reg_n) const;
    void write(const uint32_t, uint8_t);

    void reg_display() const;
    void reg_debug(uint8_t reg_n) const;
};

#endif //__REGISTER_H__