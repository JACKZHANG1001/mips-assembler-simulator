//
// Created by Jack on 2021/3/16.
//

#include "cpu.h"
using namespace std;

REG REGISTERS;
uint32_t PC;
int32_t HI;
int32_t LO;

void init_cpu() {
    // TODO: fix the initial status
    REGISTERS[$zero] = bitset<32>(0).to_string();
    REGISTERS[$at] = bitset<32>(0).to_string();
    REGISTERS[$v0] = bitset<32>(0).to_string();
    REGISTERS[$v1] = bitset<32>(0).to_string();
    REGISTERS[$a0] = bitset<32>(0).to_string();
    REGISTERS[$a1] = bitset<32>(0).to_string();
    REGISTERS[$a2] = bitset<32>(0).to_string();
    REGISTERS[$a3] = bitset<32>(0).to_string();
    REGISTERS[$t0] = bitset<32>(0).to_string();
    REGISTERS[$t1] = bitset<32>(0).to_string();
    REGISTERS[$t2] = bitset<32>(0).to_string();
    REGISTERS[$t3] = bitset<32>(0).to_string();
    REGISTERS[$t4] = bitset<32>(0).to_string();
    REGISTERS[$t5] = bitset<32>(0).to_string();
    REGISTERS[$t6] = bitset<32>(0).to_string();
    REGISTERS[$t7] = bitset<32>(0).to_string();
    REGISTERS[$s0] = bitset<32>(0).to_string();
    REGISTERS[$s1] = bitset<32>(0).to_string();
    REGISTERS[$s2] = bitset<32>(0).to_string();
    REGISTERS[$s3] = bitset<32>(0).to_string();
    REGISTERS[$s4] = bitset<32>(0).to_string();
    REGISTERS[$s5] = bitset<32>(0).to_string();
    REGISTERS[$s6] = bitset<32>(0).to_string();
    REGISTERS[$s7] = bitset<32>(0).to_string();
    REGISTERS[$t8] = bitset<32>(0).to_string();
    REGISTERS[$t9] = bitset<32>(0).to_string();
    REGISTERS[$k0] = bitset<32>(0).to_string();
    REGISTERS[$k1] = bitset<32>(0).to_string();
    REGISTERS[$gp] = bitset<32>(0).to_string();
    REGISTERS[$sp] = bitset<32>(0).to_string();
    REGISTERS[$fp] = bitset<32>(0).to_string();
    REGISTERS[$ra] = bitset<32>(0).to_string();
    PC = 0;
    HI = 0;
    LO = 0;
}

void load(REGISTER reg, int32_t content) {
    if (!IS_RUNNING) {
        init_cpu();
        init_memory();
    }
    REGISTERS[reg] = bitset<32>(content).to_string();
}

void store(REGISTER reg, uint32_t addr) {
    if(!IS_RUNNING) {
        init_cpu();
        init_memory();
    }
    MEMORY[addr] = REGISTERS[reg];
}

void r_instruction(const string & bin_str) {

}

void i_instruction(const string & bin_str) {

}

void j_instruction(const string & bin_str) {

}