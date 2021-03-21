//
// Created by Jack on 2021/3/18.
//

#ifndef MIPS_ASSEMBLER_SIMULATOR_IO_H
#define MIPS_ASSEMBLER_SIMULATOR_IO_H

#include <iostream>
#include <fstream>
using namespace std;

extern ifstream INPUT1;
extern ifstream INPUT2;
extern ofstream OUTPUT;

ifstream & read_first_file(char* filename1);

ifstream & read_second_file(char* filename2);

ofstream & write_output_file(char* filename3);

#endif //MIPS_ASSEMBLER_SIMULATOR_IO_H
