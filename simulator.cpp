#include <cassert>
#include <functional>
//#include <elf.h>
//#include <gelf.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <chrono>

#include "simulator.h"
#include "decoder.h"

void Simulator::load(const char* name)
{
//    int fd = 0;
//    Elf *e;
//    size_t n;
//    GElf_Phdr phdr;
//    GElf_Shdr shdr;
//    FILE* elf = fopen(name, "rb");
//
//    if (elf_version(EV_CURRENT) == EV_NONE)
//        errx(EX_SOFTWARE, " ELF library initialization failed: %s ", elf_errmsg(-1));
//
//    if ((fd = open(name, O_RDONLY, 0)) < 0)
//        err(EX_NOINPUT, " open \"%s \" failed ", "tmp");
//
//    if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
//        errx(EX_SOFTWARE, " elf_begin () failed: %s . ", elf_errmsg(-1));
//
//    if (elf_kind(e) != ELF_K_ELF)
//        errx(EX_DATAERR, " \"%s \" is not an ELF object . ", "tmp");
//
//    if (elf_getphdrnum(e, &n) != 0)
//        errx(EX_DATAERR, " elf_getphdrnum() failed: %s. ", elf_errmsg(-1));
//
//    for (int k = 0; k < n; k++)
//    {
//        if (gelf_getphdr(e, k, &phdr) != &phdr)
//            errx(EX_SOFTWARE, " getphdr() failed: %s . ",
//                 elf_errmsg(-1));
//
//        if (phdr.p_type == PT_LOAD)
//        {
//            fseek(elf, phdr.p_offset, SEEK_SET);
//            uint8_t* buf = (uint8_t*)calloc(phdr.p_memsz, sizeof(uint8_t));
//            fread(buf, sizeof(uint8_t), phdr.p_filesz, elf);
//            riscv.memory.write(phdr.p_vaddr, buf, phdr.p_memsz);
//            free(buf);
//        }
//
//    }
//
//    Elf_Scn* scn = NULL;
//
//    while ((scn = elf_nextscn(e, scn)) != NULL) {
//        gelf_getshdr(scn, &shdr);
//        if (shdr.sh_type == SHT_SYMTAB) {
//            break;
//        }
//    }
//
//    uint32_t startPc = 0;
//    int count = 0;
//    Elf_Data* data = elf_getdata(scn, NULL);
//    count = shdr.sh_size / shdr.sh_entsize;
//    for (int ii = 0; ii < count; ++ii)
//    {
//        GElf_Sym sym;
//        gelf_getsym(data, ii, &sym);
//        if(!strcmp(elf_strptr(e, shdr.sh_link, sym.st_name), "main"))
//            startPc = sym.st_value;
//    }
//
//    PCReg pcreg = {startPc};
//    pc.set(pcreg);
}



Simulator::Simulator(uint32_t memory_size)
    : memory(memory_size)
{
    imem.attachMemory(&memory);
}

void Simulator::run()
{
    memory_reg.setNop();
    execute_reg.setNop();
    decode_reg.setNop();
    fetch_reg.setNop();

    uint32_t num_executed = 0;
    auto t_start = std::chrono::high_resolution_clock::now();

    while (true)
    {
        doWriteBack();
        doMemory();
        doExecute();
        doDecode();
        doFetch();

        if(fetch_reg.get().encoding == 0)
            break;
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_total = t_end - t_start;

    std::cout << "Total instructions: " << num_executed << std::endl
              << "Total time: " << t_total.count() << " ms" << std::endl;
}

void Simulator::doFetch()
{
    Imem::Input in = { .a = pc.get().pc };
    Imem::Output out = imem.operate(in);

    DecodeReg current_decoding = getDecoding();
    if (stall_condition(current_decoding))
        return;

    FetchReg freg = {
        .pc = pc.get().pc,
        .encoding = out.d};
    fetch_reg.set(freg);

    PCReg preg;
    ExecuteReg ereg = execute_reg.get();
    DecodeReg dreg = decode_reg.get();
    if (ereg.flags.insType == I_BR && ereg.cmp_res)
    {
        preg.pc = ereg.alu_res;
        fetch_reg.setNop();
    }
    else if (dreg.flags.insType == I_JAL)
    {
        preg.pc = dreg.pc + (dreg.imm << 1);
        fetch_reg.setNop();
    }
    else if (ereg.flags.insType == I_JALR)
    {
        preg.pc = ereg.alu_res;
        fetch_reg.setNop();
    }
    else
        preg.pc += 4;
    pc.set(preg);
}

DecodeReg Simulator::getDecoding()
{
    FetchReg freg = fetch_reg.get();

    Decoder decoder;
    Decoder::Decoding decoding = decoder.getDecoding(freg.encoding);

    Decoder::Type type = decoder.instrType[decoding.opcode];
    RegFile::Input in = {
        .a1 = decoding.rs1,
        .a2 = decoding.rs2};
    in.wb_we = 0;

    RegFile::Output out = regfile.operate(in);
    DecodeReg dreg;

    dreg.pc = freg.pc;
    dreg.rs1 = in.a1;
    dreg.rs2 = in.a2;
    dreg.rd = decoding.rd;
    dreg.d1 = out.d1;
    dreg.d2 = out.d2;
    dreg.imm = decoding.getImm(type);
    dreg.flags = decoding.flags;

    return dreg;
}

void Simulator::doDecode()
{
    ExecuteReg ex = execute_reg.get();
    if ((ex.flags.insType == I_BR && ex.cmp_res) ||
        ex.flags.insType == I_JALR || fetch_reg.isNop())
    {
        decode_reg.setNop();
        return;
    }
    // TODO: Bypasses
    DecodeReg dreg = getDecoding();

    if (stall_condition(dreg))
    {
        decode_reg.setNop();
        return;
    }
    const auto& ereg = execute_reg.get();
    const auto& mreg = memory_reg.get();

    if (memory_bypass(dreg) == 1)
        dreg.d1 = mreg.wb_d;
    else if (memory_bypass(dreg) == 2)
        dreg.d2 = mreg.wb_d;

    if (execute_bypass(dreg) == 1)
        dreg.d1 = ereg.alu_res;
    else if (execute_bypass(dreg) == 2)
        dreg.d2 = ereg.alu_res;

    decode_reg.set(dreg);
}

void Simulator::doExecute()
{
    if (decode_reg.isNop())
    {
        execute_reg.setNop();
        return;
    }

    DecodeReg dreg = decode_reg.get();
    int32_t src1 = dreg.d1;
    int32_t src2 = dreg.d2; // src for alu and wd for memory
    uint32_t alu_res;
    bool cmp_res;
    auto type = dreg.flags.insType;
    auto aluOp = dreg.flags.aluOp;
    auto cmpOp = dreg.flags.cmpOp;

    if (dreg.flags.aluSrc2 == 1)
        src2 = dreg.imm;

    if (type == I_ALU || type == I_LD || type == I_ST)
    {
        switch (aluOp)
        {
            case ADD:
                alu_res = src1 + src2;
                break;
            case SUB:
                alu_res = src1 - src2;
                break;
            case XOR:
                alu_res = src1 ^ src2;
                break;
            case OR:
                alu_res = src1 | src2;
                break;
            case AND:
                alu_res = src1 & src2;
                break;
            case LT:
                alu_res = (src1 < src2) ? 1 : 0;
                break;
            case LTU:
                alu_res = ((uint32_t)src1 < (uint32_t)src2) ? 1 : 0;
                break;
            case SLL:
                alu_res = src1 << src2;
                break;
            case SRL:
                alu_res = (uint32_t)src1 >> src2;
                break;
            case SRA:
                alu_res = src1 >> src2;
                break;
            default:
                assert(0 && "Invalid AluOp");
                break;
        }
    }
    else if (type == I_BR)
    {
        switch (cmpOp)
        {
            case BEQ:
                cmp_res = (src1 == src2);
                break;
            case BNE:
                cmp_res = (src1 != src2);
                break;
            case BLT:
                cmp_res = (src1 < src2);
                break;
            case BLTU:
                cmp_res = ((uint32_t)src1 < (uint32_t)src2);
                break;
            case BGE:
                cmp_res = (src1 >= src2);
                break;
            case BGEU:
                cmp_res = ((uint32_t)src1 >= (uint32_t)src2);
                break;
            default:
                assert(0 && "Invalid CmpOp");
                break;
        }
        alu_res = dreg.pc + (dreg.imm << 1);
    }
    else if (type == I_JALR)
        alu_res = (src1 + dreg.imm) & ~1u;

    ExecuteReg out = {.pc = dreg.pc, .flags = dreg.flags, .alu_res = alu_res,
                      .cmp_res = cmp_res, .wd = src2, .rd = dreg.rd};
    execute_reg.set(out);
    // TODO: branches, signed/unsigned comparisons

}

void Simulator::doMemory()
{
    if (execute_reg.isNop())
    {
        memory_reg.setNop();
        return;
    }

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
    if (memory_reg.isNop())
        return;

    MemoryReg in = memory_reg.get();
    RegFile::Input out = {0, 0, 0, in.rd, in.wb_d, in.flags.we};
    regfile.operate(out);
}

bool Simulator::stall_condition(DecodeReg& dreg)
{
    const auto& ereg = execute_reg.get();
    bool res = ereg.flags.insType == I_LD && dreg.flags.insType != I_JAL;
    res &= (dreg.rs1 == ereg.rd) ||
           (dreg.rs2 == ereg.rd && dreg.flags.aluSrc2 == 0);

    return res;
}

int Simulator::execute_bypass(DecodeReg& dreg)
{
    const auto& ereg = execute_reg.get();
    if (dreg.flags.insType == I_JAL || !ereg.flags.we)
        return 0;
    if (dreg.rs1 == ereg.rd)
        return 1;
    if (dreg.rs2 == ereg.rd && dreg.flags.aluSrc2 == 0)
        return 2;
    return 0;
}

int Simulator::memory_bypass(DecodeReg& dreg)
{
    const auto& mreg = memory_reg.get();
    if (mreg.flags.insType != I_LD || dreg.flags.insType == I_JAL)
        return 0;
    if (dreg.rs1 == mreg.rd)
        return 1;
    if (dreg.rs2 == mreg.rd && dreg.flags.aluSrc2 == 0)
        return 2;
    return 0;
}
