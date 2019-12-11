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

    RegType get() { return data_; }
    const RegType& get() const { return data_; }
    void set(RegType& data)
    {
        data_ = data;
        isNop_ = false;
    }
    bool isNop() { return isNop_; }
    void setNop() { isNop_ = true; }

private:
    RegType data_;
    bool isNop_ = true;
};

struct PCReg
{
    address_t pc = 0;
};

struct FetchReg
{
    address_t pc = 0;
    uint32_t encoding = 0;
};

struct DecodeReg
{
    address_t pc = 0;
    flags_t flags = {};
    uint8_t rs1 = 0;
    uint8_t rs2 = 0;
    uint8_t rd = 0;     // write back reg
    uint32_t d1 = 0;    // data1
    uint32_t d2 = 0;    // data2
    uint32_t imm = 0;   // type
};

struct ExecuteReg
{
    address_t pc = 0;
    flags_t flags = {};
    uint32_t alu_res = 0;
    bool cmp_res = 0;
    uint32_t wd = 0;    // write data (for memory)
    uint8_t rd = 0;   // write back address
};

struct MemoryReg
{
    address_t pc = 0;
    flags_t flags = {};
    uint8_t rd = 0;   // write back address
    uint32_t wb_d = 0;  // write back data (alu_res or result of memory read)
    uint32_t wb_we = 0;
};

#endif
