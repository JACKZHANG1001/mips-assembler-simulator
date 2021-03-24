//
// Created by Jack on 2021/3/16.
//

#include "simulator.h"

bool IS_RUNNING = false;

int main(int argc, char** argv) {
    IS_RUNNING = true;

    // io
    char* input1 = argv[1];
    char* input2 = argv[2];
    char* input3 = argv[3];
    read_first_file(input1);
    // read_second_file(input2);
    write_output_file(input3);

    // assembling -> obj file
    vector<string> result = output(input2);
    ofstream Output;
    Output.open(OBJ);
    for (auto i = result.begin(); i != result.end(); i++) {
        Output << (*i) << endl;
    }
    Output.close();

    // init registers
    init_cpu();
    init_memory();

    // loader
    read_object(OBJ);
    load_text();
    load_data();



    // running
    INSTRUCTION_INFO temp;
    for(;;) {
        temp = read_ins(MEMORY[PC]);
        PC += 4;
        exe_ins(temp);
        if (MEMORY[PC] == "00000000000000000000000000000000") {
            cout << "Program finished." << endl;
            break;
        }
   }
    INPUT1.close();
    INPUT2.close();
    OUTPUT.close();
    return 0;


}