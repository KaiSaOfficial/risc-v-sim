#ifndef __REGISTER_H__
#define __REGISTER_H__

#include "cstdint"

class Register {
private:
    uint32_t *reg = nullptr;
    const uint16_t DEPTH = 32;

public:
    Register();

    uint32_t read(uint8_t reg_n) const;

    void write(const uint32_t, uint8_t);

    void debug(uint8_t reg_n) const;

    ~Register();
};

#endif //__REGISTER_H__