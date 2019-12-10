#include "simulator.h"

Simulator::Simulator(uint32_t memory_size)
    : memory(memory_size)
{
    i_mem.attachMemory(&memory);
}

void Simulator::doFetch() {}
void Simulator::doDecode() {}

void Simulator::doExecute()
{
    DecodeReg in = decode_reg.get();
    uint32_t src1 = in.d1;
    uint32_t src2 = in.d2; // src for alu and wd for memory
    uint32_t alu_res;

    if (in.flags.aluSrc2 == 1)
        src2 = in.imm;   // TODO: process immediate

    switch (in.flags.aluOp)
    {
        case AluOp::ADD:
            alu_res = src1 + src2;
            break;
        case AluOp::SUB:
            alu_res = src1 - src2;
            break;
        default:
            break;
    }


    ExecuteReg out = {.pc = in.pc, .flags = in.flags, .alu_res = alu_res,
                      .wd = src2, .rd = in.rd};
    execute_reg.set(out);
    // TODO: branches, signed/unsigned comparisons

}

void Simulator::doMemory()
{
    ExecuteReg in = execute_reg.get();
    uint32_t wb_d = in.alu_res;
    // alu_res is address and wb_data if no memory operation
    if (in.flags.memWe)
        memory.write(in.alu_res, &in.wd, sizeof(in.wd));  // Load
    else if ((in.flags.we == 1) && (in.flags.wbCtrl == 1))
        memory.read(in.alu_res, &wb_d, sizeof(wb_d));  // Store

    MemoryReg out = {in.pc, in.flags, in.rd, wb_d};
    memory_reg.set(out);

}

void Simulator::doWriteBack()
{
    MemoryReg in = memory_reg.get();
    RegFile::Input out = {0, 0, 0, in.rd, in.wb_d, in.flags.we};
    regfile.operate(out);
}

bool Simulator::stall_condition() {}
