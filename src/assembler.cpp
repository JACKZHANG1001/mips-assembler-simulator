//
// Created by Jack on 2021/3/16.
//

#include "ass.h"
using namespace std;

int main(int argc, char** argv) {
    if (argc > 1) {
        vector<string> result = output(argv[1]);
        ofstream output;
        output.open("OBJECT");
        for (auto i = result.begin(); i != result.end(); i++) {
            output << (*i) << endl;
        }
        output.close();
    } else {
        vector<string> result = output();
        for (auto i = result.begin(); i != result.end(); i++) {
            cout << (*i) << endl;
        }
    }

    /*
     * checking for LABEL_TABLE
    for (auto i : LABEL_TABLE) {
        cout << i.first << ": " << i.second << endl;
    }
     */
    return 0;
}
