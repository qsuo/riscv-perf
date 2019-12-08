
#include "imem.h"

Imem::Imem(Memory* memory):
    memory_(memory)
{}

Output operate(Input& input)
{
    uint32_t tmp = 0;
    memory->read(input.a, &tmp, sizeof(tmp);
    return tmp;
}
