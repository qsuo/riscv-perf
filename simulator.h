#include "imem.h"
#include "memory.h"
#include "control_reg.h"
#include "control_unit.h"
#include "regfile.h"

#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator
{
public:
    Simulator(uint32_t memory_size);

    void doFetch();
    void doDecode();
    void doExecute();
    void doMemory();
    void doWriteBack();

private:
    Memory memory;
    Imem i_mem;
    RegFile regfile;

    ControlReg<PCReg> pc;
    ControlReg<FetchReg> fetch_reg;
    ControlReg<DecodeReg> decode_reg;
    ControlReg<ExecuteReg> execute_reg;
    ControlReg<MemoryReg> memory_reg;
};

#endif // SIMULATOR_H
