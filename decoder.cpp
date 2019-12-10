
#include <iostream>
#include <cassert>

#include "decoder.h"


int Decoder::findFirstUp(uint32_t raw)
{
    int bit = 0;
    uint32_t mask = 0x1;
    int nbit = sizeof(raw) * BYTE_SIZE;
    int pos = 0;
    for(pos = 0; pos < nbit && bit == 0; pos++)
    {
        int newMask = mask << pos;
        bit = (raw & newMask) >> pos;
    }

    return --pos;
}

uint32_t Decoder::applyMask(uint32_t raw, uint32_t mask)
{
    return (raw & mask) >> this->findFirstUp(mask);
}

#define IMM12(x) (x & 0x800)
#define IMM20(x) (x & 0x80000)

#define EX12(x) (x | (0xFFFFF << 12))
#define EX20(x) (x | (0xFFF << 20))

uint32_t Decoder::Decoding::getIImm()
{
    uint32_t imm = I_imm11_0;
    if(IMM12(imm))
        imm = EX12(imm);
    return imm;
}


uint32_t Decoder::Decoding::getSImm()
{
    uint32_t imm4_0 = S_imm4_0;
    uint32_t imm11_5 = S_imm11_5 << 5;
    uint32_t imm = imm4_0 | imm11_5;

    if(IMM12(imm))
        imm = EX12(imm);

    return imm;
}

uint32_t Decoder::Decoding::getBImm()
{
    uint32_t imm12 = B_imm12 << 11;
    uint32_t imm10_5 = B_imm10_5 << 4;
    uint32_t imm4_1 = B_imm4_1;
    uint32_t imm11 = B_imm11 << 10;
    uint32_t imm = imm4_1 | imm10_5 | imm11 | imm12;
    if(IMM12(imm))
        imm = EX12(imm);
    return imm;
}

uint32_t Decoder::Decoding::getUImm()
{

    uint32_t imm = U_imm31_12;
    if(IMM20(imm))
        imm = EX20(imm);
    return imm;
}

uint32_t Decoder::Decoding::getJImm()
{
    uint32_t imm10_1 = J_imm10_1;
    uint32_t imm11 = J_imm11 << 10;
    uint32_t imm19_12 = J_imm19_12 << 11;
    uint32_t imm20 = J_imm20 << 19;
    uint32_t imm = imm10_1 | imm11 | imm19_12 | imm20;
    if(IMM20(imm))
        imm = EX20(imm);
    return imm;
}

uint32_t Decoder::Decoding::getImm(Type type)
{
    switch(type)
    {
        case I:
            return getIImm();
        case S:
            return getSImm();
        case B:
            return getBImm();
        case U:
            return getUImm();
        case J:
            return getJImm();
        default:
            return 0;
    }
}


Decoder::Decoding Decoder::getDecoding(uint32_t encoding)
{
    Decoding decoding = {
        .opcode     = applyMask(encoding, Mask::OPCODE),
        .rd         = applyMask(encoding, Mask::RD),
        .funct3     = applyMask(encoding, Mask::FUNCT3),
        .rs1        = applyMask(encoding, Mask::RS1),
        .zimm       = applyMask(encoding, Mask::ZIMM),
        .rs2        = applyMask(encoding, Mask::RS2),
        .shamt      = applyMask(encoding, Mask::SHAMT),
        .funct7     = applyMask(encoding, Mask::FUNCT7),
        .I_imm11_0  = applyMask(encoding, Mask::I_IMM11_0),
        .csr        = applyMask(encoding, Mask::CSR),
        .S_imm11_5  = applyMask(encoding, Mask::S_IMM11_5),
        .S_imm4_0   = applyMask(encoding, Mask::S_IMM4_0),
        .B_imm12    = applyMask(encoding, Mask::B_IMM12),
        .B_imm10_5  = applyMask(encoding, Mask::B_IMM10_5),
        .B_imm4_1   = applyMask(encoding, Mask::B_IMM4_1),
        .B_imm11    = applyMask(encoding, Mask::B_IMM11),
        .U_imm31_12 = applyMask(encoding, Mask::U_IMM31_12),
        .J_imm20    = applyMask(encoding, Mask::J_IMM20),
        .J_imm10_1  = applyMask(encoding, Mask::J_IMM10_1),
        .J_imm11    = applyMask(encoding, Mask::J_IMM11),
        .J_imm19_12 = applyMask(encoding, Mask::J_IMM19_12)
    };

    return decoding;
}


