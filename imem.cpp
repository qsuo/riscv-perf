
#include <cassert>

#include "imem.h"

void Imem::attachMemory(Memory* memory)
{
    assert(memory != nullptr);
    memory_ = memory;
}

Imem::Output Imem::operate(Input& input)
{
    uint32_t tmp = 0;
    memory_->read(input.a, &tmp, sizeof(tmp));
    return {tmp};
}
