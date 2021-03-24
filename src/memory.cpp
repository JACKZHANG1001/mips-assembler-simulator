//
// Created by Jack on 2021/3/17.
//

#include "memory.h"
using namespace std;

Memory MEMORY;

void init_memory() {
    for (int i = 0; i < STACK_START + 4; i += 4) {
        MEMORY[i] = bitset<32>(0).to_string();
    }
}
