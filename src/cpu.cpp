//
// Created by Jack on 2021/3/16.
//

#include "cpu.h"
#include <cmath>
using namespace std;

REG REGISTERS;
uint32_t PC;
int32_t HI;
int32_t LO;

void init_cpu() {
    // TODO: fix the initial status
    REGISTERS[$zero] = bitset<32>(0).to_string();
    REGISTERS[$at] = bitset<32>(0).to_string();
    REGISTERS[$v0] = bitset<32>(0).to_string();
    REGISTERS[$v1] = bitset<32>(0).to_string();
    REGISTERS[$a0] = bitset<32>(0).to_string();
    REGISTERS[$a1] = bitset<32>(0).to_string();
    REGISTERS[$a2] = bitset<32>(0).to_string();
    REGISTERS[$a3] = bitset<32>(0).to_string();
    REGISTERS[$t0] = bitset<32>(0).to_string();
    REGISTERS[$t1] = bitset<32>(0).to_string();
    REGISTERS[$t2] = bitset<32>(0).to_string();
    REGISTERS[$t3] = bitset<32>(0).to_string();
    REGISTERS[$t4] = bitset<32>(0).to_string();
    REGISTERS[$t5] = bitset<32>(0).to_string();
    REGISTERS[$t6] = bitset<32>(0).to_string();
    REGISTERS[$t7] = bitset<32>(0).to_string();
    REGISTERS[$s0] = bitset<32>(0).to_string();
    REGISTERS[$s1] = bitset<32>(0).to_string();
    REGISTERS[$s2] = bitset<32>(0).to_string();
    REGISTERS[$s3] = bitset<32>(0).to_string();
    REGISTERS[$s4] = bitset<32>(0).to_string();
    REGISTERS[$s5] = bitset<32>(0).to_string();
    REGISTERS[$s6] = bitset<32>(0).to_string();
    REGISTERS[$s7] = bitset<32>(0).to_string();
    REGISTERS[$t8] = bitset<32>(0).to_string();
    REGISTERS[$t9] = bitset<32>(0).to_string();
    REGISTERS[$k0] = bitset<32>(0).to_string();
    REGISTERS[$k1] = bitset<32>(0).to_string();
    REGISTERS[$gp] = bitset<32>(0).to_string();
    REGISTERS[$sp] = bitset<32>(0).to_string();
    REGISTERS[$fp] = bitset<32>(0).to_string();
    REGISTERS[$ra] = bitset<32>(0).to_string();
    PC = 0;
    HI = 0;
    LO = 0;
}

void load(REGISTER reg, int32_t content) {
    if (!IS_RUNNING) {
        init_cpu();
        init_memory();
    }
    REGISTERS[reg] = bitset<32>(content).to_string();
}

void store(REGISTER reg, uint32_t addr) {
    if(!IS_RUNNING) {
        init_cpu();
        init_memory();
    }
    MEMORY[addr] = REGISTERS[reg];
}

int32_t imm_sign_extend(const string & immediate) {
    // 16-bit signed integer to 32-bit signed integer
    int32_t temp;
    if (immediate[0] == '0')
        temp = stoi(immediate);
    else
        temp = stoi(immediate.substr(1, immediate.length()-1))
                - pow(2, 15);
    return temp;
}

int32_t sign_num(const string & num) {
    int32_t temp;
    if (num[0] == '0')
        temp = stoi(num);
    else temp = stoi(num.substr(1, 31))
                - pow(2, 31);
    return temp;
}

INSTRUCTION_INFO read_ins(const string & bin_str) {
    INSTRUCTION_INFO result;
    uint OP = stoi(bin_str.substr(0, 6));
    uint RS = stoi(bin_str.substr(6, 5));
    uint RT = stoi(bin_str.substr(11, 5));
    uint RD = stoi(bin_str.substr(16, 5));;
    uint SHAMT = stoi(bin_str.substr(21, 5));
    uint FUNCT = stoi(bin_str.substr(26, 6));
    int ETC = 0;
    result.op = OP;
    result.rs = RS;
    result.rt = RT;
    result.rd = RD;
    result.shamt = SHAMT;
    result.funct = FUNCT;
    result.etc = ETC;
    switch (OP) {
        case 0:
            switch (FUNCT) {
                case 0:
                    result.name = SLL;
                    result.type = R;
                    return result;
                    break;
                case 2:
                    result.name = SRL;
                    result.type = R;
                    return result;
                    break;
                case 3:
                    result.name = SRA;
                    result.type = R;
                    return result;
                    break;
                case 4:
                    result.name = SLLV;
                    result.type = R;
                    return result;
                    break;
                case 6:
                    result.name = SRLV;
                    result.type = R;
                    return result;
                    break;
                case 7:
                    result.name = SRAV;
                    result.type = R;
                    return result;
                    break;
                case 8:
                    result.name = JR;
                    result.type = R;
                    return result;
                    break;
                case 9:
                    result.name = JALR;
                    result.type = R;
                    return result;
                    break;
                case 12:
                    result.name = SYSCALL;
                    result.type = R;
                    return result;
                    break;
                case 16:
                    result.name = MFHI;
                    result.type = R;
                    return result;
                    break;
                case 17:
                    result.name = MTHI;
                    result.type = R;
                    return result;
                    break;
                case 18:
                    result.name = MFLO;
                    result.type = R;
                    return result;
                    break;
                case 19:
                    result.name = MTLO;
                    result.type = R;
                    return result;
                    break;
                case 24:
                    result.name = MULT;
                    result.type = R;
                    return result;
                    break;
                case 25:
                    result.name = MULTU;
                    result.type = R;
                    return result;
                    break;
                case 26:
                    result.name = DIV;
                    result.type = R;
                    return result;
                    break;
                case 27:
                    result.name = DIVU;
                    result.type = R;
                    return result;
                    break;
                case 32:
                    result.name = ADD;
                    result.type = R;
                    return result;
                    break;
                case 33:
                    result.name = ADDU;
                    result.type = R;
                    return result;
                    break;
                case 34:
                    result.name = SUB;
                    result.type = R;
                    return result;
                    break;
                case 35:
                    result.name = SUBU;
                    result.type = R;
                    return result;
                    break;
                case 36:
                    result.name = AND;
                    result.type = R;
                    return result;
                    break;
                case 37:
                    result.name = OR;
                    result.type = R;
                    return result;
                    break;
                case 38:
                    result.name = XOR;
                    result.type = R;
                    return result;
                    break;
                case 39:
                    result.name = NOR;
                    result.type = R;
                    return result;
                    break;
                case 42:
                    result.name = SLT;
                    result.type = R;
                    return result;
                    break;
                case 43:
                    result.name = SLTU;
                    result.type = R;
                    return result;
                    break;
                case 48:
                    result.name = TGE;
                    result.type = R;
                    return result;
                    break;
                case 49:
                    result.name = TGEU;
                    result.type = R;
                    return result;
                    break;
                case 50:
                    result.name = TLT;
                    result.type = R;
                    return result;
                    break;
                case 51:
                    result.name = TLTU;
                    result.type = R;
                    return result;
                    break;
                case 52:
                    result.name = TEQ;
                    result.type = R;
                    return result;
                    break;
                case 54:
                    result.name = TNE;
                    result.type = R;
                    return result;
                    break;
                default:
                    return result;
            }
            break;
        case 1:
            switch (RT) {
                case 0:
                    result.name = BLTZ;
                    result.type = I;
                    ETC = imm_sign_extend(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                case 1:
                    result.name = BGEZ;
                    result.type = I;
                    ETC = imm_sign_extend(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                case 8:
                    result.name = TGEI;
                    result.type = I;
                    ETC = imm_sign_extend(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                case 9:
                    result.name = TGEIU;
                    result.type = I;
                    ETC = stoi(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                case 10:
                    result.name = TLTI;
                    result.type = I;
                    ETC = imm_sign_extend(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                case 11:
                    result.name = TLTIU;
                    result.type = I;
                    ETC = stoi(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                case 14:
                    result.name = TNEI;
                    result.type = I;
                    ETC = imm_sign_extend(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                case 16:
                    result.name = BLTZAL;
                    result.type = I;
                    ETC = imm_sign_extend(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                case 17:
                    result.name = BGEZAL;
                    result.type = I;
                    ETC = imm_sign_extend(bin_str.substr(16, 16));
                    result.etc = ETC;
                    return result;
                    break;
                default:
                    return result;
            }
            break;
        case 2:
            result.name = j;
            result.type = J;
            ETC = stoi(bin_str.substr(6, 26));
            result.etc = ETC;
            return result;
            break;
        case 3:
            result.name = JAL;
            result.type = J;
            ETC = stoi(bin_str.substr(6, 26));
            result.etc = ETC;
            return result;
            break;
        case 4:
            result.name = BEQ;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 5:
            result.name = BNE;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 6:
            result.name = BLEZ;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 7:
            result.name = BGTZ;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 8:
            result.name = ADDI;
            result.type = I;
            // careful for sign-extension
            ETC = stoi(bin_str.substr((16, 16)));
            result.etc = ETC;
            return result;
            break;
        case 9:
            result.name = ADDIU;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 10:
            result.name = SLTI;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 11:
            result.name = SLTIU;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 12:
            result.name = ANDI;
            result.type = I;
            // ANDI is zero-extended
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 13:
            result.name = ORI;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 14:
            result.name = XORI;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 15:
            // load imm to the upper halfword of register rt, the lower
            // are all set to 0
            result.name = LUI;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 28:
            switch (FUNCT) {
                case 0:
                    result.name = MADD;
                    result.type = R;
                    return result;
                    break;
                case 1:
                    result.name = MADDU;
                    result.type = R;
                    return result;
                    break;
                case 2:
                    result.name = MUL;
                    result.type = R;
                    return result;
                    break;
                case 4:
                    result.name = MSUB;
                    result.type = R;
                    return result;
                    break;
                case 5:
                    result.name = MSUBU;
                    result.type = R;
                    return result;
                    break;
                case 32:
                    result.name = CLZ;
                    result.type = R;
                    return result;
                    break;
                case 33:
                    result.name = CLO;
                    result.type = R;
                    return result;
                    break;
            }
            break;
        case 32:
            result.name = LB;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 33:
            result.name = LH;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 34:
            result.name = LWL;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 35:
            result.name = LW;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 36:
            result.name = LBU;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 37:
            result.name = LHU;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 38:
            result.name = LWR;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 40:
            result.name = SB;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 41:
            result.name = SH;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 42:
            result.name = SWL;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 43:
            result.name = SW;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 46:
            result.name = SWR;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 48:
            result.name = LL;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        case 56:
            result.name = SC;
            result.type = I;
            ETC = imm_sign_extend(bin_str.substr(16, 16));
            result.etc = ETC;
            return result;
            break;
        default:
            return result;

    }
}

void exe_ins(const INSTRUCTION_INFO & ins) {
    REGISTER RS;
    REGISTER RT;
    REGISTER RD;
    switch (ins.op) {
        case ADD:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            REGISTERS[RD] = bitset<32>(
                            sign_num(REGISTERS[RS]) +
                            sign_num(REGISTERS[RT])).to_string();
            return;
            break;
        case ADDU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            REGISTERS[RD] = bitset<32>(
                            stoi(REGISTERS[RS]) +
                            stoi(REGISTERS[RT])).to_string();
            return;
            break;
        case ADDI:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            REGISTERS[RT] = bitset<32>(
                            sign_num(REGISTERS[RS]) +
                            ins.etc).to_string();
            return;
            break;
        case ADDIU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            REGISTERS[RT] = bitset<32>(
                            stoi(REGISTERS[RS]) +
                            ins.etc).to_string();
            return;
            break;
        case AND:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            REGISTERS[RD] = bitset<32>(
                            stoi(REGISTERS[RS]) &
                            stoi(REGISTERS[RT])).to_string();
            return;
            break;
        case ANDI:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            REGISTERS[RT] = bitset<32>(
                            stoi(REGISTERS[RS]) &
                            ins.etc).to_string();
            return;
            break;
        case CLO:
            RS = static_cast<REGISTER>(ins.rs);
            RD = static_cast<REGISTER>(ins.rd);
            break;
        case CLZ:
            break;
        case DIV:
            break;
        case DIVU:
            break;
        case MULT:
            break;
        case MULTU:
            break;
        case MUL:
            break;
        case MADD:
            break;
        case MADDU:
            break;
        case MSUB:
            break;
        case MSUBU:
            break;
        case NOR:
            break;
        case OR:
            break;
        case ORI:
            break;
        case SLL:
            break;
        case SLLV:
            break;
        case SRA:
            break;
        case SRAV:
            break;
        case SRL:
            break;
        case SRLV:
            break;
        case SUB:
            break;
        case SUBU:
            break;
        case XOR:
            break;
        case XORI:
            break;
        case LUI:
            break;
        case SLT:
            break;
        case SLTU:
            break;
        case SLTI:
            break;
        case SLTIU:
            break;
        case BEQ:
            break;
        case BGEZ:
            break;
        case BGEZAL:
            break;
        case BGTZ:
            break;
        case BLEZ:
            break;
        case BLTZAL:
            break;
        case BLTZ:
            break;
        case BNE:
            break;
        case j:
            break;
        case JAL:
            break;
        case JALR:
            break;
        case JR:
            break;
        case TEQ:
            break;
        case TEQI:
            break;
        case TNE:
            break;
        case TNEI:
            break;
        case TGE:
            break;
        case TGEU:
            break;
        case TGEI:
            break;
        case TGEIU:
            break;
        case TLT:
            break;
        case TLTU:
            break;
        case TLTI:
            break;
        case TLTIU:
            break;
        case LB:
            break;
        case LBU:
            break;
        case LH:
            break;
        case LHU:
            break;
        case LW:
            break;
        case LWL:
            break;
        case LWR:
            break;
        case LL:
            break;
        case SB:
            break;
        case SH:
            break;
        case SW:
            break;
        case SWL:
            break;
        case SWR:
            break;
        case SC:
            break;
        case MFHI:
            break;
        case MFLO:
            break;
        case MTHI:
            break;
        case MTLO:
            break;
        case SYSCALL:
            break;
    }
}