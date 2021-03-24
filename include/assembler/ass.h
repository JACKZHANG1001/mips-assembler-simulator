//
// Created by Jack on 2021/3/1.
//

#ifndef MIPS_ASSEMBLER_SIMULATOR_ASS_H
#define MIPS_ASSEMBLER_SIMULATOR_ASS_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdint>
#include <map>
#include <list>
#include <bitset>

using namespace std;

extern const uint32_t TEXT_BASE_ADDR; // global text-base
extern map<string,uint32_t> LABEL_TABLE; // global label table
// list can be helpful when merging two lines
extern list<string> LINES; // global lines
extern list<string> DATA;
extern vector<vector<string> > TOKENS; // global TOKENS

enum INSTRUCTION_TYPE { R, I, J };
enum REGISTER {
    $zero = 0,     // 0
    $at,       // 1
    $v0,       // 2
    $v1,       // 3
    $a0,       // 4
    $a1,       // 5
    $a2,       // 6
    $a3,       // 7
    $t0,       // 8
    $t1,       // 9
    $t2,       // 10
    $t3,       // 11
    $t4,       // 12
    $t5,       // 13
    $t6,       // 14
    $t7,       // 15
    $s0,       // 16
    $s1,       // 17
    $s2,       // 18
    $s3,       // 19
    $s4,       // 20
    $s5,       // 21
    $s6,       // 22
    $s7,       // 23
    $t8,       // 24
    $t9,       // 25
    $k0,       // 26
    $k1,       // 27
    $gp,       // 28
    $sp,       // 29
    $fp,       // 30
    $ra        // 31
};
enum INSTRUCTION {  // op:hex, funct:hex
    ADD,         // r, op: 00, funct: 20
    ADDU,        // r, op: 00, funct: 21
    ADDI,        // i, op: 08
    ADDIU,       // r, op: 09
    AND,         // r, op: 00, funct: 24
    ANDI,        // r, op: 0c
    CLO,         // r, op: 1c, funct: 21
    CLZ,         // r, op: 1c, funct: 20
    DIV,         // r, op: 00, funct: 1a
    DIVU,        // r, op: 00, funct: 1b
    MULT,        // r, op: 00, funct: 18
    MULTU,       // r, op: 00, funct: 19
    MUL,         // r, op: 1c, funct: 02
    MADD,        // r, op: 1c, funct: 00
    MADDU,       // r, op: 1c, funct: 01
    MSUB,        // r, op: 1c, funct: 04
    MSUBU,       // r, op: 1c, funct: 05
    NOR,         // r, op: 00, funct: 27
    OR,          // r, op: 00, funct: 25
    ORI,         // i, op: 0d
    SLL,         // r, op: 00, funct: 00
    SLLV,        // r, op: 00, funct: 04
    SRA,         // r, op: 00, funct: 03
    SRAV,        // r, op: 00, funct: 37
    SRL,         // r, op: 00, funct: 02
    SRLV,        // r, op: 00, funct: 06
    SUB,         // r, op: 00, funct: 22
    SUBU,        // r, op: 00, funct: 23
    XOR,         // r, op: 00, funct: 26
    XORI,        // i, op: 0e
    LUI,         // i, op: 0f
    SLT,         // r, op: 00, funct: 2a
    SLTU,        // r, op: 00, funct: 2b
    SLTI,        // i, op: 0a
    SLTIU,       // i, op: 0b
    BEQ,         // i, op: 04
    BGEZ,        // i, op: 01 *
    BGEZAL,      // i, op: 01 *
    BGTZ,        // i, op: 07
    BLEZ,        // i, op: 06
    BLTZAL,      // i, op: 01 *
    BLTZ,        // i, op: 01 *
    BNE,         // i, op: 05
    j,           // j, op: 02
    JAL,         // g, op: 03
    JALR,        // r, op: 00, funct: 09
    JR,          // r, op: 00, funct: 08
    TEQ,         // r, op: 00, funct: 34
    TEQI,        // i, op: 01 *
    TNE,         // r, op: 00, funct: 36
    TNEI,        // i, op: 01 *
    TGE,         // r, op: 00, funct: 30
    TGEU,        // r, op: 00, funct: 31
    TGEI,        // i, op: 01 *
    TGEIU,       // i, op: 01 *
    TLT,         // r, op: 00, funct: 32
    TLTU,        // r, op: 00, funct: 33
    TLTI,        // i, op: 01 *
    TLTIU,       // i, op: 01 *
    LB,          // i, op: 20
    LBU,         // i, op: 24
    LH,          // i, op: 21
    LHU,         // i, op: 25
    LW,          // i, op: 23
    LWL,         // i, op: 22
    LWR,         // i, op: 26
    LL,          // i, op: 30
    SB,          // i, op: 28
    SH,          // i, op: 29
    SW,          // i, op: 2b
    SWL,         // i, op: 2a
    SWR,         // i, op: 2e
    SC,          // i, op: 38
    MFHI,        // r, op: 00, funct: 10
    MFLO,        // r, op: 00, funct: 12
    MTHI,        // r, op: 00, funct: 11
    MTLO,        // r, op: 00, funct: 13
    SYSCALL      // r, op: 00, funct: 0c
};
struct INSTRUCTION_INFO {
    INSTRUCTION name;
    INSTRUCTION_TYPE type;
    uint32_t op;
    uint32_t rs;
    uint32_t rt;
    uint32_t rd;
    uint32_t shamt;
    uint32_t funct;
    int32_t etc;
};

// does not work
void read_text(istream & input);

void read_text(ifstream & input, char* filename);

void read_data(istream & input);

void read_data(ifstream & input, char* filename);

void clean_comment(list<string> & lines);

INSTRUCTION_INFO get_instruction_info(vector<string> & line, uint32_t loc);

map<string,uint32_t> get_label_table(list<string> & lines, bool is_data = false);

vector<string> split(string &line);

string info_to_binary(INSTRUCTION_INFO &);

vector<vector<string> > tokenizer(list<string> & lines = LINES);

vector<string> translate(vector<vector<string> > & tokens = TOKENS);

vector<string> output();

vector<string> output(char* filename);

#endif


