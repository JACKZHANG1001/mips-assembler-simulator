//
// Created by Jack on 2021/3/18.
//

#ifndef MIPS_ASSEMBLER_SIMULATOR_LOADER_H
#define MIPS_ASSEMBLER_SIMULATOR_LOADER_H

#define OBJ "OBJECT"
#include "ass.h"
#include "memory.h"
#include "cpu.h"
using namespace std;

/*
 * The format of the OBJECT file:
 *
 *
 * --------------------------------
 * bin
 * bin
 * ...
 * bin
 * .data
 * mips directive
 * ...
 * mips directive
 * ---------------------------------
 */

extern vector<string> TEXT_SEG;
extern vector<string> DATA_SEG;

void read_object(char* filename);

void load_text();

void load_data();



#endif //MIPS_ASSEMBLER_SIMULATOR_LOADER_H
