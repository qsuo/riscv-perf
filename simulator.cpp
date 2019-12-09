#include "simulator.h"

Simulator::Simulator(uint32_t memory_size)
    : memory(memory_size)
{
    i_mem.attachMemory(&memory);
}


void Simulator::doFetch() {}
void Simulator::doDecode() {}
void Simulator::doExecute() {}
void Simulator::doMemory() {
    ExecuteReg in = execute_reg.get();
    uint32_t wb_d = in.alu_res;
    // alu_res is address and wb_data if no memory operation
    if (in.flags.memWe)
        memory.write(in.alu_res, &in.wd, sizeof(in.wd));  // Load
    else if ((in.flags.we == 1) && (in.flags.wbCtrl == 1))
        memory.read(in.alu_res, &wb_d, sizeof(wb_d));  // Store

    MemoryReg out = {in.pc, in.flags, in.wb_a, wb_d};
    memory_reg.set(out);

}

void Simulator::doWriteBack()
{
    MemoryReg in = memory_reg.get();
    RegFile::Input out = {0, 0, 0, in.wb_a, in.wb_d, in.flags.we};
    regfile.operate(out);
}


