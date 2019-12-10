
#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <unordered_map>

enum Opcode
{
    OP_IMM      = 0b0010011,
    OP          = 0b0110011,
    LUI         = 0b0110111,
    AUIPC       = 0b0010111,
    JAL         = 0b1101111,
    JALR        = 0b1100111,
    BRANCH      = 0b1100011,
    LOAD        = 0b0000011,
    STORE       = 0b0100011,
    MISC_MEM    = 0b0001111,
    SYSTEM      = 0b1110011
};

enum AluOp { ADD, SUB };
enum InType { ALU, LD, ST, JMP, BR };

class ControlUnit
{
public:

    struct Input
    {
        uint32_t encoding;
    };

    struct Output
    {
        int aluOp;
        int aluSrc2;
        int brnCond;
        int memWe;
        int we;
        int wbCtrl;
    };

    using map = std::unordered_map<uint32_t, Output>;

    Output operate(Input& t);
};

#endif
