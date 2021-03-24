//
// Created by Jack on 2021/3/17.
//

#ifndef MIPS_ASSEMBLER_SIMULATOR_MEMORY_H
#define MIPS_ASSEMBLER_SIMULATOR_MEMORY_H

#define MEMORY_SIZE 0x600000
#define TEXT_START 0x400000
#define DATA_START 0x500000
#define DATA_END 0x600000
#define STACK_START 0xa00000

#include "ass.h"
using namespace std;


typedef map<uint32_t, string> Memory;
extern Memory MEMORY;

void init_memory();


#endif //MIPS_ASSEMBLER_SIMULATOR_MEMORY_H
