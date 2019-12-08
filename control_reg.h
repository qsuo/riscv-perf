#include <cstdint>

#include "control_unit.h"

#ifndef CONTROL_REG_H
#define CONTROL_REG_H

using flags_t = ControlUnit::Output;

template <class RegType>
class ControlReg
{
public:
    ControlReg(RegType& data) : data_(data) {}

    RegType getData() const { return data_; }
    setData(RegType& data) { data_ = data; };

private:
    RegType data_;
};

struct FetchReg
{
    uint32_t encoding;
};

struct DecodeReg
{
    flags_t flags;
    uint8_t wb_a;   // write back address
    uint32_t d1;    // data
    uint32_t d2;    // data
    bool se;        // sign extension
    uint32_t imm; // type ?
};

struct ExecuteReg
{
    flags_t flags;
    uint32_t alu_res;
    uint32_t wd;    // write data (for memory)
    uint8_t wb_a;
};

struct MemoryReg
{
    flags_t flags;
    uint8_t wb_a;
    uint32_t wb_d;  // write back address
};

#endif
