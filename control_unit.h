
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

enum AluOp { ADD = 0b000,
             XOR = 0b100,
             OR  = 0b110,
             AND = 0b111,
             LT  = 0b010,
             LTU = 0b011,
             SLL = 0b001,
             SRL = 0b101,
             SUB = 0b1000,
             SRA = 0b1001};

enum CmpOp { BEQ  = 0b000,
             BNE  = 0b001,
             BLT  = 0b100,
             BLTU = 0b110,
             BGE  = 0b101,
             BGEU = 0b111 };

enum InsType { I_ALU, I_LD, I_ST, I_JAL, I_JALR, I_BR };

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
        int cmpOp;
        int aluSrc2;
        int memWe;
        int we;
        int wbCtrl;
        int insType;
    };

    using map = std::unordered_map<uint32_t, Output>;

    Output operate(Input& t);
};

#endif
