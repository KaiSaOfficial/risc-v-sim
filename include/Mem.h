#ifndef __MEM_H__
#define __MEM_H__

#include "cstdint"

class Mem {
private:
    uint8_t *ptr = nullptr;      // ptr of Mem
    const uint16_t DEPTH = 2048; // Depth of Mem

public:
    Mem(void);

    uint8_t read(uint16_t) const;

    void write(uint16_t, uint8_t);

    void debug(uint16_t Address) const;

    ~Mem();
};

#endif //__MEM_H__