//
// Created by Jack on 2021/3/16.
//

#ifndef MIPS_ASSEMBLER_SIMULATOR_CPU_H
#define MIPS_ASSEMBLER_SIMULATOR_CPU_H

#include "ass.h"
#include "memory.h"
#include "simulator.h"
using namespace std;

typedef map<REGISTER, string> REG;

// this contains the contents of the registers
extern REG REGISTERS;
extern uint32_t PC;
extern int32_t HI;
extern int32_t LO;

void init_cpu();

void load(REGISTER reg, int32_t content);

void store(REGISTER reg, uint32_t addr);

void r_instruction(const string & bin_str);

void i_instruction(const string & bin_str);

void j_instruction(const string & bin_str);


#endif //MIPS_ASSEMBLER_SIMULATOR_CPU_H
