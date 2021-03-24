//
// Created by Jack on 2021/3/18.
//

#include "loader.h"
char OBJ[] = "OBJECT\0";

using namespace std;

vector<string> TEXT_SEG;
vector<string> DATA_SEG;

void read_object(char* filename) {
    ifstream input;
    input.open(filename);
    string line = "";
    while (getline(input, line)) {
        if (line.find(".data") != string::npos)
            break;
        TEXT_SEG.push_back(line);
    }
    while (getline(input, line)) {
        DATA_SEG.push_back(line);
    }
    input.close();
}

void load_text() {
    uint32_t pos = TEXT_START;
    for (auto text : TEXT_SEG) {
        MEMORY[pos] = norm_to_little(text);
        pos += 4;
    }
}

string half_reverse(const string & str) {
    // 16
    // "1234" -> "3412"
    string temp;
    temp += str.substr(8, 8);
    temp += str.substr(0, 8);
    return temp;
}

void load_data() {
    uint32_t pos = DATA_START;
    vector<string> temp;
    uint32_t byte_loc = 0;
    uint32_t half_loc = 0;
    uint32_t word_loc = 0;
    for (auto data : DATA_SEG) {
        temp = split(data);
        if (temp[0] == ".byte") {
            byte_loc = pos % 4;
            word_loc = pos - byte_loc;
            if (byte_loc == 0)
                MEMORY[word_loc].replace(24, 8,temp[1]);
            else if (byte_loc == 1)
                MEMORY[word_loc].replace(16, 8, temp[1]);
            else if (byte_loc == 2)
                MEMORY[word_loc].replace(8, 8, temp[1]);
            else if (byte_loc == 3)
                MEMORY[word_loc].replace(0, 8, temp[1]);
            pos += 1;
        } else if (temp[0] == ".half") {
            byte_loc = pos % 4;
            half_loc = pos % 2;
            word_loc = pos - pos / 4 * 4;
            if (byte_loc == 0)
                MEMORY[word_loc].replace(16, 16, half_reverse(temp[1]));
            else if (byte_loc == 1) {
                pos += 1;
                byte_loc += 1;
                MEMORY[word_loc].replace(0, 16, half_reverse(temp[1]));
            } else if (byte_loc == 2)
                MEMORY[word_loc].replace(0, 16, half_reverse(temp[1]));
            else if (byte_loc == 3) {
                pos += 1;
                byte_loc = 0;
                word_loc += 4;
                MEMORY[word_loc].replace(16, 16, half_reverse(temp[1]));
            }
            pos += 2;
        } else if (temp[0] == ".word") {
            byte_loc = pos % 4;
            word_loc = pos - pos / 4 * 4;
            if (byte_loc != 0) {
                word_loc += 4;
                pos = word_loc;
                byte_loc = 0;
                MEMORY[word_loc] = norm_to_little(temp[1]);
            } else if (byte_loc == 0) {
                MEMORY[word_loc] = norm_to_little(temp[1]);
            }
            pos += 4;
        }
    }
}