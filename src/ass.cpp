//
// Created by Jack on 2021/3/2.
//

#include "ass.h"
using namespace std;

/*
 * TODO: in the main function,
 *       use
 *       int main(int argc, char** argv)
 *       to handle reading problem
 *
 *       .text & .data ? in reading part
 */

const uint32_t TEXT_BASE_ADDR = 0x400000;
map<string, uint32_t> LABEL_TABLE;
list<string> LINES;
list<string> DATA;
vector<vector<string> > TOKENS;

void read_text(istream & input) {
    // read from stdin
    string temp = "";
    bool empty_flag = false;
    while (getline(input, temp)) {
        if (temp.find(".text") != string::npos)
                break;
    }
    while (getline(input, temp)) {
        if (temp.find(".data") != string::npos)
            break;
        empty_flag = false;
        if (temp.length() == 0)
            empty_flag = true;
        for (auto i = 0; i < temp.length(); i++) {
            if (isalpha(temp[i]) ||
                isalnum(temp[i]) ||
                temp[i] == '_' ||
                temp[i] == '.') {

                empty_flag = false;
                break;
            } else {
                empty_flag = true;
            }
        }
        if (!empty_flag)
            LINES.push_back(temp);
    }
}

void read_text(ifstream & input, char* filename) {
    // read from files
    string temp = "";
    bool empty_flag = false;
    input.open(filename);
    while (getline(input, temp)) {
        if (temp.find(".text") != string::npos)
            break;
    }
    while (getline(input, temp)) {
        if (temp.find(".data") != string::npos)
            break;
        empty_flag = false;
        if (temp.length() == 0)
            empty_flag = true;
        for (auto i = 0; i < temp.length(); i++) {
            if (isalpha(temp[i]) ||
                isalnum(temp[i]) ||
                temp[i] == '_' ||
                temp[i] == '.') {

                empty_flag = false;
                break;
            } else {
                empty_flag = true;
            }
        }
        if (!empty_flag)
            LINES.push_back(temp);
    }
    input.close();
}

void read_data(istream & input) {
    // read from stdin
    string temp = "";
    bool empty_flag = false;
    while (getline(input, temp)) {
        if (temp.find(".data") != string::npos)
            break;
    }
    while (getline(input, temp)) {
        if (temp.find(".text") != string::npos)
            break;
        empty_flag = false;
        if (temp.length() == 0)
            empty_flag = true;
        for (auto i = 0; i < temp.length(); i++) {
            if (isalpha(temp[i]) ||
                isalnum(temp[i]) ||
                temp[i] == '_' ||
                temp[i] == '.') {

                empty_flag = false;
                break;
            } else {
                empty_flag = true;
            }
        }
        if (!empty_flag)
            DATA.push_back(temp);
    }
}

void read_data(ifstream & input, char* filename) {
    // read from files
    string temp = "";
    bool empty_flag = false;
    input.open(filename);
    while (getline(input, temp)) {
        if (temp.find(".data") != string::npos)
            break;
    }
    while (getline(input, temp)) {
        if (temp.find(".text") != string::npos)
            break;
        empty_flag = false;
        if (temp.length() == 0)
            empty_flag = true;
        for (auto i = 0; i < temp.length(); i++) {
            if (isalpha(temp[i]) ||
                isalnum(temp[i]) ||
                temp[i] == '_' ||
                temp[i] == '.') {

                empty_flag = false;
                break;
            } else {
                empty_flag = true;
            }
        }
        if (!empty_flag)
            DATA.push_back(temp);
    }
    input.close();
}

void clean_comment(list<string> & lines) {
    for (auto line_pr = lines.begin(); line_pr != lines.end(); line_pr++) {
        string temp = "";
        int quote_loc = 0;
        for (auto i = 0; i < line_pr->length(); i++) {
            if ((*line_pr)[i] == ':') {
                quote_loc = i;
                break;
            }
            else
                quote_loc = -1;
        }
        if (quote_loc == -1) {
            bool flag1 = true; // TODO: this may be redundant
            for (auto i = 0; i < line_pr->length(); i++) {
                if (flag1 && !isspace((*line_pr)[i])
                          && isspace((*line_pr)[i+1])) {
                    flag1 = false;
                    (*line_pr)[i+1] = ',';
                    break;
                }
            }
        }
        else {
            bool flag2 = false;
            for (auto i = 0; i < line_pr->length(); i++) {
                if (!isspace((*line_pr)[i]) && isspace((*line_pr)[i+1])) {
                    flag2 = true;
                    for (auto j = i + 1; j < line_pr->length(); j++) {
                        if (flag2 && !isspace((*line_pr)[j])
                            && isspace((*line_pr)[j+1])) {
                            flag2 = false;
                            (*line_pr)[j+1] = ',';
                            break;
                        }
                    }
                    break;
                }
            }
        }
        // above achieves
        // ____label:___instruction____.... -> ____label:___instruction,__....
        // OR
        // ___instruction____.... -> ___instruction,__....
        for (auto i = 0; i < line_pr->length(); i++) {
            // clean comments and whitespaces


            // for .ascii or .asciiz
            if ((*line_pr)[i] == '"') {
                temp += '"';
                for (auto j = i + 1; j < line_pr->length(); j++) {
                    if ((*line_pr)[j] == '"') {
                        temp += (*line_pr)[j];
                        break;
                    }
                    temp += (*line_pr)[j];
                }
                break;
            }
            if (isspace((*line_pr)[i])) continue;
            else if ((*line_pr)[i] != '#')
                temp += (*line_pr)[i];
            else
                break;
        }
        // empty line detected
        if (temp.length() == 0) {
            auto temp_pr = line_pr;
            line_pr--;
            lines.erase(temp_pr);
            continue;
        }
        *line_pr = temp;
    }
}

list<string>::iterator merging_lines(list<string>::iterator p1,
                                     list<string>::iterator p2,
                                     list<string>& lines) {
    // merging p2 tp p1 and erasing p2
    // helping function
    *p1 = (*p1) + (*p2);
    lines.erase(p2);
    return p1;
}

string releasing_label(string &line) {
    // label1:addi$s1,$s2,$s3
    // addi$s1,$s2,$s3
    bool flag = false;
    string temp = "";
    for (auto i = 0; i < line.length(); i++) {
        if (line[i] == ':') {
            flag = true;
            continue;
        }
        if (!flag)
            continue;
        else
            temp += line[i];
    }
    return temp;
}

map<string,uint32_t> get_label_table(list<string> & lines) {
    // merging lines and release labels
    // case1
    // label1: expression
    // case2
    // label2:
    // expression
    // case3
    // expression
    uint32_t line_no = 0;
    uint32_t addr;
    // case1: flag = true
    // case2: flag = false
    bool flag = false;
    string label_to_add = "";

    for (auto line_pr = lines.begin(); line_pr != lines.end(); line_pr++) {
        line_no = 0;
        auto temp_pr = lines.begin();
        while (temp_pr != line_pr) {
            line_no++;
            temp_pr++;
        }
        string temp = *line_pr;
        for (auto i = 0; i < temp.length(); i++) {
            if (temp[i] == ':') {
                // label found
                // reset label_to_add
                label_to_add = "";
                for (auto k = 0; k < i; k++) {
                    label_to_add += temp[k];
                }
                LABEL_TABLE[label_to_add] = line_no;
                continue;
            }
            if (label_to_add.length() != 0 &&
                    (isalpha(temp[i]) || temp[i] == '_')) {
                // case1
                // called only if there is something
                // behind the :
                flag = true;
                label_to_add = "";
                *line_pr = releasing_label(*line_pr);
                break;
                // addition completed
                // go to the next line
            }
        }
        if (label_to_add == "")
            continue;
        else {
            flag = false;
            // the next line must be an instruction
            label_to_add = "";
            merging_lines(line_pr, (++line_pr), lines);
            --line_pr;
            *line_pr = releasing_label(*line_pr);
        }
    }
    return LABEL_TABLE;
}

vector<string> split(string &line) {
    // help function
    // add,$s1,$s2,$s3
    // "add" "$s1" ...
    vector<string> result;
    auto ch_pr = line.begin();
    for (auto ptr = line.begin(); ptr != line.end(); ptr++) {
        if ((*ptr) == ',') {
            result.push_back(string(ch_pr, ptr));
            ptr++;
            ch_pr = ptr;
        }
    }
    result.push_back(string(ch_pr, line.end()));
    return result;
}

vector<vector<string> > tokenizer(list<string> & lines) {
    for (auto line = lines.begin(); line != lines.end(); line++) {
        TOKENS.push_back(split((*line)));
    }
    return TOKENS;
}

int branch_label_trans(vector<string> & line, uint32_t loc) {
    // help function
    // bne,rs,rt,label -> bne,rs,rt,ADDR
    // TODO: loc ? dealing with the TEXT_BASE_ADDR
    auto label_pr = line.end() - 1;
    auto addr_pr = LABEL_TABLE.find((*label_pr));
    auto label_loc = addr_pr->second;
    auto offset = label_loc - (loc + 1); // PC + 4
    return offset;
}

int jump_label_trans(vector<string> & line, uint32_t loc) {
    // help function
    auto label_pr = line.end() - 1;
    auto addr_pr = LABEL_TABLE.find((*label_pr));
    auto label_loc = addr_pr->second + TEXT_BASE_ADDR / 4; // # (line_num + 0x400000 / 4) word
    auto offset = label_loc;
    return offset;
}

pair<int, int> load_store_addr_trans(vector<string> & line, uint32_t loc) {
    // rs, offset
    pair<int, int> addr;
    auto addr_pr = line.end() - 1;
    string temp = *addr_pr;
    string rs;
    string offset;
    string::iterator temp_pr;
    for (auto pr = temp.begin(); pr != temp.end(); pr++) {
        if ((*pr) == '(') {
            offset = string(temp.begin(), pr);
            temp_pr = pr + 1;
        }
        if ((*pr) == ')')
            rs = string(temp_pr, pr);
    }
    int num_offset = stoi(offset);
    int num_rs = 0;
    char rs_2 = rs[1];
    switch (rs_2) {
        case 'a': {
            if (rs[2] == 't')
                num_rs = 1;
            if (rs[2] == '0')
                num_rs = 4;
            if (rs[2] == '1')
                num_rs = 5;
            if (rs[2] == '2')
                num_rs = 6;
            if (rs[2] == '3')
                num_rs = 7;
            break;
        }
        case 'v': {
            if (rs[2] == '0')
                num_rs = 2;
            if (rs[2] == '1')
                num_rs = 3;
            break;
        }
        case 't': {
            if (rs[2] == '0')
                num_rs = 8;
            if (rs[2] == '1')
                num_rs = 9;
            if (rs[2] == '2')
                num_rs = 10;
            if (rs[2] == '3')
                num_rs = 11;
            if (rs[2] == '4')
                num_rs = 12;
            if (rs[2] == '5')
                num_rs = 13;
            if (rs[2] == '6')
                num_rs = 14;
            if (rs[2] == '7')
                num_rs = 15;
            if (rs[2] == '8')
                num_rs = 24;
            if (rs[2] == '9')
                num_rs = 25;
            break;
        }
        case 's': {
            if (rs[2] == '0')
                num_rs = 16;
            if (rs[2] == '1')
                num_rs = 17;
            if (rs[2] == '2')
                num_rs = 18;
            if (rs[2] == '3')
                num_rs = 19;
            if (rs[2] == '4')
                num_rs = 20;
            if (rs[2] == '5')
                num_rs = 21;
            if (rs[2] == '6')
                num_rs = 22;
            if (rs[2] == '7')
                num_rs = 23;
            if (rs[2] == 'p')
                num_rs = 29;
            break;
        }
        case 'k': {
            if (rs[2] == '0')
                num_rs = 26;
            if (rs[2] == '1')
                num_rs = 27;
            break;
        }
        case 'g': {
            num_rs = 28;
            break;
        }
        case 'f': {
            num_rs = 30;
            break;
        }
        case 'r': {
            num_rs = 31;
            break;
        }
        default:
            break;
    }
    addr.first = num_rs;
    addr.second = num_offset;
    return addr;
}

REGISTER get_register(string & str) {
    // help function
    if (str == "$zero")
        return $zero;
    if (str == "$at")
        return $at;
    if (str == "$v0")
        return $v0;
    if (str == "$v1")
        return $v1;
    if (str == "$a0")
        return $a0;
    if (str == "$a1")
        return $a1;
    if (str == "$a2")
        return $a2;
    if (str == "$a3")
        return $a3;
    if (str == "$t0")
        return $t0;
    if (str == "$t1")
        return $t1;
    if (str == "$t2")
        return $t2;
    if (str == "$t3")
        return $t3;
    if (str == "$t4")
        return $t4;
    if (str == "$t5")
        return $t5;
    if (str == "$t6")
        return $t6;
    if (str == "$t7")
        return $t7;
    if (str == "$s0")
        return $s0;
    if (str == "$s1")
        return $s1;
    if (str == "$s2")
        return $s2;
    if (str == "$s3")
        return $s3;
    if (str == "$s4")
        return $s4;
    if (str == "$s5")
        return $s5;
    if (str == "$s6")
        return $s6;
    if (str == "$s7")
        return $s7;
    if (str == "$t8")
        return $t8;
    if (str == "$t9")
        return $t9;
    if (str == "$k0")
        return $k0;
    if (str == "$k1")
        return $k1;
    if (str == "$gp")
        return $gp;
    if (str == "$sp")
        return $sp;
    if (str == "$fp")
        return $fp;
    if (str == "$ra")
        return $ra;
}

INSTRUCTION_INFO get_instruction_info(vector<string> & line, uint32_t loc) {
    // line is the tokenized vector<string>
    // TODO: etc? str -> int?
    // TODO: store, load

    string ins = line[0];
    INSTRUCTION_INFO temp;
    if (ins == "add") {
       temp.name = ADD;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x20;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "addu") {
       temp.name = ADDU;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x21;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "addi") {
       temp.name = ADDI;
       temp.type = I;
       temp.op = 0x08;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[1]);
       temp.etc = stoi(line[3]);
    }
     if (ins == "addiu") {
       temp.name = ADDIU;
       temp.type = I;
       temp.op = 0x09;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[1]);
       temp.etc = stoi(line[3]);
    }
     if (ins == "and") {
       temp.name = AND;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x24;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "andi") {
       temp.name = ANDI;
       temp.type = I;
       temp.op = 0x0c;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[1]);
       temp.etc = stoi(line[3]);
    }
     if (ins == "clo") {
       temp.name = CLO;
       temp.type = R;
       temp.op = 0x1c;
       temp.shamt = 0;
       temp.funct = 0x21;
       temp.rs = get_register(line[2]);
       temp.rt = 0x00;
       temp.rd = get_register(line[1]);
    }
     if (ins == "clz") {
       temp.name = CLZ;
       temp.type = R;
       temp.op = 0x1c;
       temp.shamt = 0;
       temp.funct = 0x20;
       temp.rs = get_register(line[2]);
       temp.rt = 0x00;
       temp.rd = get_register(line[1]);
    }
     if (ins == "div") {
       temp.name = DIV;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x1a;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "divu") {
       temp.name = DIVU;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x1b;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "mult") {
       temp.name = MULT;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x18;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "multu") {
       temp.name = MULTU;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x19;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "mul") {
       temp.name = MUL;
       temp.type = R;
       temp.op = 0x1c;
       temp.shamt = 0;
       temp.funct = 0x02;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "madd") {
       temp.name = MADD;
       temp.type = R;
       temp.op = 0x1c;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "maddu") {
       temp.name = MADDU;
       temp.type = R;
       temp.op = 0x1c;
       temp.shamt = 0;
       temp.funct = 0x01;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "msub") {
       temp.name = MSUB;
       temp.type = R;
       temp.op = 0x1c;
       temp.shamt = 0;
       temp.funct = 0x04;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "msubu") {
       temp.name = MSUBU;
       temp.type = R;
       temp.op = 0x1c;
       temp.shamt = 0;
       temp.funct = 0x05;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "nor") {
       temp.name = NOR;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x27;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "or") {
       temp.name = OR;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x25;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "ori") {
       temp.name = ORI;
       temp.type = I;
       temp.op = 0x0d;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[1]);
       temp.etc = stoi(line[3]);
    }
     if (ins == "sll") {
       temp.name = SLL;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = stoi(line[3]);
       temp.funct = 0x00;
       temp.rs = 0; // ignored
       temp.rt = get_register(line[2]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "sllv") {
       temp.name = SLLV;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x04;
       temp.rs = get_register(line[3]);
       temp.rt = get_register(line[2]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "sra") {
       temp.name = SRA;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = stoi(line[3]);
       temp.funct = 0x03;
       temp.rs = 0;
       temp.rt = get_register(line[2]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "srav") {
       temp.name = SRAV;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x07;
       temp.rs = get_register(line[3]);
       temp.rt = get_register(line[2]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "srl") {
       temp.name = SRL;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = stoi(line[3]);
       temp.funct = 0x02;
       temp.rs = 0;
       temp.rt = get_register(line[2]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "srlv") {
       temp.name = SRLV;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x06;
       temp.rs = get_register(line[3]);
       temp.rt = get_register(line[2]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "sub") {
       temp.name = SUB;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x22;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "subu") {
       temp.name = SUBU;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x23;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "xor") {
       temp.name = XOR;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x26;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "xori") {
       temp.name = XORI;
       temp.type = I;
       temp.op = 0x0e;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = stoi(line[3]);
    }
     if (ins == "lui") {
       temp.name = LUI;
       temp.type = I;
       temp.op = 0x0f;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = 0;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = stoi(line[2]);
    }
     if (ins == "slt") {
       temp.name = SLT;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x2a;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "sltu") {
       temp.name = SLTU;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x2b;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[3]);
       temp.rd = get_register(line[1]);
    }
     if (ins == "slti") {
       temp.name = SLTI;
       temp.type = I;
       temp.op = 0x0a;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = stoi(line[3]);
    }
     if (ins == "sltiu") {
       temp.name = SLTIU;
       temp.type = I;
       temp.op = 0x0b;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[2]);
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = stoi(line[3]);
    }
     if (ins == "beq") {
       temp.name = BEQ;
       temp.type = I;
       temp.op = 0x04;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
       temp.etc = branch_label_trans(line, loc);
    }
     if (ins == "bgez") {
       temp.name = BGEZ;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x01;
       temp.rd = 0;
       temp.etc = branch_label_trans(line, loc);
    }
     if (ins == "bgezal") {
       temp.name = BGEZAL;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x11;
       temp.rd = 0;
       temp.etc = branch_label_trans(line, loc);
    }
     if (ins == "bgtz") {
       temp.name = BGTZ;
       temp.type = I;
       temp.op = 0x07;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0;
       temp.rd = 0;
       temp.etc = branch_label_trans(line, loc);
    }
     if (ins == "blez") {
       temp.name = BLEZ;
       temp.type = I;
       temp.op = 0x06;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0;
       temp.rd = 0;
       temp.etc = branch_label_trans(line, loc);
    }
     if (ins == "bltzal") {
       temp.name = BLTZAL;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x10;
       temp.rd = 0;
       temp.etc = branch_label_trans(line, loc);
    }
     if (ins == "bltz") {
       temp.name = BLTZ;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0;
       temp.rd = 0;
       temp.etc = branch_label_trans(line, loc);
    }
     if (ins == "bne") {
       temp.name = BNE;
       temp.type = I;
       temp.op = 0x05;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
       temp.etc = branch_label_trans(line, loc);
    }
     if (ins == "j") {
       temp.name = j;
       temp.type = J;
       temp.op = 0x02;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.etc = jump_label_trans(line, loc);
    }
     if (ins == "jal") {
       temp.name = JAL;
       temp.type = J;
       temp.op = 0x03;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.etc = jump_label_trans(line, loc);
    }
     if (ins == "jalr") {
       temp.name = JALR;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x09;
       temp.rs = get_register(line[1]);
       temp.rt = 0;
       temp.rd = get_register(line[2]);
    }
     if (ins == "jr") {
       temp.name = JR;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x08;
       temp.rs = get_register(line[1]);
       temp.rt = 0;
       temp.rd = 0;
    }
     if (ins == "teq") {
       temp.name = TEQ;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x34;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "teqi") {
       temp.name = TEQI;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x0c;
       temp.rd = 0;
       temp.etc = stoi(line[2]);
    }
     if (ins == "tne") {
       temp.name = TNE;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x36;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "tnei") {
       temp.name = TNEI;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x0e;
       temp.rd = 0;
       temp.etc = stoi(line[2]);
    }
     if (ins == "tge") {
       temp.name = TGE;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x30;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "tgeu") {
       temp.name = TGEU;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x31;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "tgei") {
       temp.name = TGEI;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x08;
       temp.rd = 0;
       temp.etc = stoi(line[2]);
    }
     if (ins == "tgeiu") {
       temp.name = TGEIU;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x09;
       temp.rd = 0;
       temp.etc = stoi(line[2]);
    }
     if (ins == "tlt") {
       temp.name = TLT;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x32;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "tltu") {
       temp.name = TLTU;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x33;
       temp.rs = get_register(line[1]);
       temp.rt = get_register(line[2]);
       temp.rd = 0;
    }
     if (ins == "tlti") {
       temp.name = TLTI;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x0a;
       temp.rd = 0;
       temp.etc = stoi(line[2]);
    }
     if (ins == "tltiu") {
       temp.name = TLTIU;
       temp.type = I;
       temp.op = 0x01;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = get_register(line[1]);
       temp.rt = 0x0b;
       temp.rd = 0;
       temp.etc = stoi(line[2]);
    }
     if (ins == "lb") {
       pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = LB;
       temp.type = I;
       temp.op = 0x20;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "lbu") {
       pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = LBU;
       temp.type = I;
       temp.op = 0x24;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "lh") {
       pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = LH;
       temp.type = I;
       temp.op = 0x21;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "lhu") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = LHU;
       temp.type = I;
       temp.op = 0x25;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "lw") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = LW;
       temp.type = I;
       temp.op = 0x23;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "lwl") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = LWL;
       temp.type = I;
       temp.op = 0x22;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "lwr") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = LWR;
       temp.type = I;
       temp.op = 0x26;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "ll") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = LL;
       temp.type = I;
       temp.op = 0x30;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "sb") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = SB;
       temp.type = I;
       temp.op = 0x28;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "sh") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = SH;
       temp.type = I;
       temp.op = 0x29;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "sw") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = SW;
       temp.type = I;
       temp.op = 0x2b;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "swl") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = SWL;
       temp.type = I;
       temp.op = 0x2a;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "swr") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = SWR;
       temp.type = I;
       temp.op = 0x2e;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "sc") {
         pair<int, int> addr = load_store_addr_trans(line, loc);
       temp.name = SC;
       temp.type = I;
       temp.op = 0x38;
       temp.shamt = 0;
       temp.funct = 0x00;
       temp.rs = addr.first;
       temp.rt = get_register(line[1]);
       temp.rd = 0;
       temp.etc = addr.second;
    }
     if (ins == "mfhi") {
       temp.name = MFHI;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x10;
       temp.rs = 0;
       temp.rt = 0;
       temp.rd = get_register(line[1]);
    }
     if (ins == "mflo") {
       temp.name = MFLO;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x12;
       temp.rs = 0;
       temp.rt = 0;
       temp.rd = get_register(line[1]);
    }
     if (ins == "mthi") {
       temp.name = MTHI;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x11;
       temp.rs = get_register(line[1]);
       temp.rt = 0;
       temp.rd = 0;
    }
     if (ins == "mtlo") {
       temp.name = MTLO;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x13;
       temp.rs = get_register(line[1]);
       temp.rt = 0;
       temp.rd = 0;
    }
     if (ins == "syscall") {
       temp.name = SYSCALL;
       temp.type = R;
       temp.op = 0x00;
       temp.shamt = 0;
       temp.funct = 0x0c;
       temp.rs = 0;
       temp.rt = 0;
       temp.rd = 0;
    }
    return temp;
}

string info_to_binary(INSTRUCTION_INFO & info) {
    INSTRUCTION_TYPE type = info.type;
    string op = bitset<6>(info.op).to_string();
    string rs = bitset<5>(info.rs).to_string();
    string rt = bitset<5>(info.rt).to_string();
    string rd = bitset<5>(info.rd).to_string();
    string shamt = bitset<5>(info.shamt).to_string();
    string funct = bitset<6>(info.funct).to_string();
    string etc = "";
    if (type == R) {
       return op + rs + rt + rd + shamt + funct;
    }
    if (type == I) {
        etc = bitset<16>(info.etc).to_string();
        return op + rs + rt + etc;
    }
    if (type == J) {
        etc = bitset<26>(info.etc).to_string();
        return op + etc;
    }
}

vector<string> translate(vector<vector<string> > & tokens) {
    vector<string> result;
    for (auto token = tokens.begin(); token != tokens.end(); token++) {
        uint32_t loc = token - tokens.begin();
        INSTRUCTION_INFO temp = get_instruction_info((*token), loc);
        string bi_ins = info_to_binary(temp);
        result.push_back(bi_ins);
    }
    return result;
}

vector<string> output() {
    read_text(cin);
    read_data(cin);
    clean_comment(LINES);
    clean_comment(DATA);
    get_label_table(LINES);
    get_label_table(DATA);
    tokenizer();
    vector<string> result = translate();
    result.push_back(".data");
    for (auto i : DATA) {
        result.push_back(i);
    }
    return result;
}

vector<string> output(char* filename) {
    ifstream input;
    read_text(input, filename);
    read_data(input, filename);
    clean_comment(LINES);
    clean_comment(DATA);
    get_label_table(LINES);
    get_label_table(DATA);
    tokenizer();
    vector<string> result = translate();
    result.push_back(".data");
    for (auto i : DATA) {
        result.push_back(i);
    }
    return result;
}

