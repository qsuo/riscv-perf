
#include "memory.h"

#include <iostream>
#include <cassert>
#include <cstring>

Memory::Memory(uint32_t size):
    size_(size)
{
    memory_ = new uint8_t[size_];
    assert(memory_ != NULL);
}

Memory::~Memory()
{
    delete[] memory_;
    memory_ = NULL;
}

void Memory::read(address_t address, void* dst, std::size_t nbyte)
{
    assert((address < size_ + nbyte) && "Memory read out of bounds");
    std::memcpy(dst, &memory_[address], nbyte);
}

void Memory::write(address_t address, void* src, std::size_t nbyte)
{
    assert((address < size_ + nbyte) && "Memory write out of bounds");
    std::memcpy(&memory_[address], src, nbyte);
}


