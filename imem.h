
#ifndef I_MEMORY_H
#define I_MEMORY_H

#include "memory.h"

class Imem
{

public:

    struct Input
    {
        address_t a;
    };

    struct Output
    {
        uint32_t d;
    };

    Imem() {}
    void attachMemory(Memory* memory);

    Output operate(Input& input);

private:
    Memory* memory_ = nullptr;
};

#endif  // I_MEMORY_H
