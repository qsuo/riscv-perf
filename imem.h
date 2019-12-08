
#ifndef MEMORY_H
#define MEMORY_H

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
    }

    Imem(Memory* memory);
    void attachMemory(Memory* memory) {};

    Output operate(Input& input);

private:
    Memory* memory_;
};

#endif
