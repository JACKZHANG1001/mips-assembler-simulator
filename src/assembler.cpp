//
// Created by Jack on 2021/3/16.
//

#include "ass.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc > 1) {
        ofstream output;
        ifstream input;
        read_mips(input, argv[1]);
        clean_comment(LINES);
        LABEL_TABLE = get_label_table(LINES);
        tokenizer();
        vector<string> result = translate();
        output.open("OUTPUT");
        for (auto i = result.begin(); i != result.end(); i++) {
            output << (*i) << endl;
        }
        output.close();
    } else {
        read_mips(cin);
        clean_comment(LINES);
        LABEL_TABLE = get_label_table(LINES);
        tokenizer();
        vector<string> result = translate();
        for (auto i = result.begin(); i != result.end(); i++) {
            cout << (*i) << endl;
        }
    }
    return 0;
}
