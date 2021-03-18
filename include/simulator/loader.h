//
// Created by Jack on 2021/3/18.
//

#ifndef MIPS_ASSEMBLER_SIMULATOR_LOADER_H
#define MIPS_ASSEMBLER_SIMULATOR_LOADER_H

#include "ass.h"
using namespace std;

extern const vector<string> TEXT;
extern const vector<string> DATA;

vector<string> read_data(ifstream & input, char* filename);



#endif //MIPS_ASSEMBLER_SIMULATOR_LOADER_H
