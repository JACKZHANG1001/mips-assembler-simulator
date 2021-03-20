//
// Created by Jack on 2021/3/18.
//

#include "loader.h"

using namespace std;

vector<string> TEXT_SEG;
vector<string> DATA_SEG;

void read_object(ifstream & input, char* filename = OBJ) {
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
}

void load_text() {

}

void load_data() {

}