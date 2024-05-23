#ifndef __REGISTER_H__
#define __REGISTER_H__

#include "cstdint"

typedef enum { zero, rd, rs1, rs2 } Reg_T;

class Register {
private:
    uint32_t reg;
    Reg_T type;

public:
    Register(Reg_T type_);

    uint32_t read(void) const;

    void write(uint32_t);

    void debug(uint16_t) const;
};

#endif //__REGISTER_H__