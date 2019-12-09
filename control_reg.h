#include <cstdint>

#include "control_unit.h"

#ifndef CONTROL_REG_H
#define CONTROL_REG_H

using address_t = uint32_t;
using flags_t = ControlUnit::Output;

template <class RegType>
class ControlReg
{
public:
    ControlReg() {}
    ControlReg(RegType& data) : data_(data) {}

    RegType get() const { return data_; }
    void set(RegType& data) { data_ = data; };

private:
    RegType data_;
};

struct PCReg
{
    address_t pc;
};

struct FetchReg
{
    address_t pc;
    uint32_t encoding;
};

struct DecodeReg
{
    address_t pc;
    flags_t flags;
    uint8_t wb_a;   // write back address
    uint32_t d1;    // data
    uint32_t d2;    // data
    bool se;        // sign extension
    uint32_t imm;   // type ?
};

struct ExecuteReg
{
    address_t pc;
    flags_t flags;
    uint32_t alu_res;
    uint32_t wd;    // write data (for memory)
    uint8_t wb_a;   // write back address
};

struct MemoryReg
{
    address_t pc;
    flags_t flags;
    uint8_t wb_a;   // write back address
    uint32_t wb_d;  // write back data (alu_res or result of memory read)
};

#endif
