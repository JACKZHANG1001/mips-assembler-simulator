//
// Created by Jack on 2021/3/18.
//

#include "syscall.h"
#include <stdlib.h>
using namespace std;

void syscall() {
    int v0 = stoi(norm_to_little(REGISTERS[$v0]), nullptr, 2);
    string a0 = norm_to_little(REGISTERS[$a0]);
    string a1 = norm_to_little(REGISTERS[$a1]);
    string a2 = norm_to_little(REGISTERS[$a2]);
    string temp;
    uint32_t word_loc = 0;
    OUTPUT.flush();
    switch (v0) {
        case 1:
            OUTPUT << stoi(a0, nullptr, 2);
            return;
            break;
        case 4:
            word_loc = stoi(a0, nullptr, 2);
            for (;;) {
                temp = norm_to_little(MEMORY[word_loc]);
                for(int i = 0; i < 4; i++) {
                    if (temp.substr(i * 8, 8) == "00000000")
                        return;
                    OUTPUT << char(stoi(temp.substr(i * 8, 8), nullptr, 2));
                }
                word_loc += 4;
            }
            break;
        case 5:
            getline(INPUT1, temp);
            REGISTERS[$v0] = norm_to_little(bitset<32>(stoi(temp, nullptr, 10)).to_string());
            return;
            break;
        case 8:
            break;
        case 9:
            // allocate bytes
            break;
        case 10:
            exit(EXIT_SUCCESS);
            break;
        case 11:
            OUTPUT << char(stoi(a0.substr(24, 8), nullptr, 2));
            return;
            break;
        case 12:
            getline(INPUT1, temp);
            REGISTERS[$v0] = norm_to_little(bitset<32>(int(temp[0])).to_string());
            return;
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
        case 16:
            break;
        case 17:
            _Exit(stoi(norm_to_little(REGISTERS[$a0]), nullptr, 2));
    }
}
