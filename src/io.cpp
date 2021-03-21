//
// Created by Jack on 2021/3/18.
//

#include "io.h"
ifstream INPUT1;
ifstream INPUT2;
ofstream OUTPUT;


ifstream & read_first_file(char* filename1) {
    INPUT1.open(filename1);
    return INPUT1;
}

ifstream & read_second_file(char* filename2) {
    INPUT2.open(filename2);
    return INPUT2;
}

ofstream & write_output_file(char* filename3) {
    OUTPUT.open(filename3);
    return OUTPUT;
}

