//
// Created by Jack on 2021/3/18.
//

#include "loader.h"

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

void load_data() {

}