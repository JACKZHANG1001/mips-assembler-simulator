//
// Created by Jack on 2021/3/16.
//

#include "cpu.h"

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
    REGISTERS[$sp] = bitset<32>(STACK_START).to_string();
    REGISTERS[$fp] = bitset<32>(0).to_string();
    REGISTERS[$ra] = bitset<32>(0).to_string();
    PC = TEXT_START;
    HI = 0;
    LO = 0;
}

string norm_to_little(const string &norm_str) {
    string temp = "";
    temp += norm_str.substr(24, 8);
    temp += norm_str.substr(16, 8);
    temp += norm_str.substr(8, 8);
    temp += norm_str.substr(0, 8);
    return temp;

}

void load(REGISTER reg, int32_t content) {
    if (!IS_RUNNING) {
        init_cpu();
        init_memory();
    }
    REGISTERS[reg] = norm_to_little(
            bitset<32>(content).to_string());
}

void store(REGISTER reg, uint32_t addr) {
    if (!IS_RUNNING) {
        init_cpu();
        init_memory();
    }
    MEMORY[addr] = norm_to_little(REGISTERS[reg]);
}

int32_t imm_sign_extend(const string &immediate) {
    // 16-bit signed integer to 32-bit signed integer
    int32_t temp;
    if (immediate[0] == '0')
        temp = stoi(immediate, nullptr, 2);
    else
        temp = stoi(immediate.substr(1, immediate.length() - 1))
               - pow(2, 15);
    return temp;
}

int32_t sign_num(const string &num) {
    int32_t temp;
    if (num[0] == '1')
        temp = stoi(num.substr(1, 31), nullptr, 2)
               - pow(2, 31);
    else
        temp = stoi(num, nullptr, 2);
    return temp;
}

INSTRUCTION_INFO read_ins(const string &little) {
    INSTRUCTION_INFO result;
    string bin_str = norm_to_little(little);
    uint32_t OP = stoi(bin_str.substr(0, 6), nullptr, 2);
    uint32_t RS = stoi(bin_str.substr(6, 5), nullptr, 2);
    uint32_t RT = stoi(bin_str.substr(11, 5), nullptr, 2);
    uint32_t RD = stoi(bin_str.substr(16, 5), nullptr, 2);
    uint32_t SHAMT = stoi(bin_str.substr(21, 5), nullptr, 2);
    uint32_t FUNCT = stoi(bin_str.substr(26, 6), nullptr, 2);
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
                    ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
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
                    ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
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
            ETC = stoi(bin_str.substr(6, 26), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 3:
            result.name = JAL;
            result.type = J;
            ETC = stoi(bin_str.substr(6, 26), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 4:
            result.name = BEQ;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 5:
            result.name = BNE;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 6:
            result.name = BLEZ;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 7:
            result.name = BGTZ;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 8:
            result.name = ADDI;
            result.type = I;
            // careful for sign-extension
            ETC = stoi(bin_str.substr((16, 16)), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 9:
            result.name = ADDIU;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
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
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 12:
            result.name = ANDI;
            result.type = I;
            // ANDI is zero-extended
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
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
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
            result.etc = ETC;
            return result;
            break;
        case 37:
            result.name = LHU;
            result.type = I;
            ETC = stoi(bin_str.substr(16, 16), nullptr, 2);
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

void exe_ins(const INSTRUCTION_INFO &ins) {
    REGISTER RS;
    REGISTER RT;
    REGISTER RD;
    string temp_rs, temp_rt, temp_rd;
    int count = 0;
    int64_t temp_mult = 0;
    uint64_t temp_multu = 0;
    int word_loc, byte_loc;
    switch (ins.name) {
        case ADD:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(sign_num(temp_rs) + sign_num(temp_rt)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case ADDU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(stoi(temp_rs, nullptr, 2) + stoi(temp_rt, nullptr, 2)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case ADDI:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = bitset<32>(sign_num(temp_rs) + ins.etc).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case ADDIU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = bitset<32>(stoi(temp_rs, nullptr, 2) + ins.etc).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case AND:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(sign_num(temp_rs) & stoi(temp_rt, nullptr, 2)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case ANDI:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = bitset<32>(sign_num(temp_rs) & ins.etc).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case CLO:
            RS = static_cast<REGISTER>(ins.rs);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            count = 0;
            for (auto i = 0; i < temp_rs.length(); i++) {
                if (temp_rs[i] == '1')
                    count++;
                else
                    break;
            }
            temp_rd = bitset<32>(count).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case CLZ:
            RS = static_cast<REGISTER>(ins.rs);
            RD = static_cast<REGISTER>(ins.rd);
            /*
            const uint32_t numIntBits = sizeof(uint32_t) * 8;
            uint32_t x = stoi(REGISTERS[RS], nullptr, 2);
            x |= x >> 1;
            x |= x >> 2;
            x |= x >> 4;
            x |= x >> 8;
            x |= x >> 16;

            x -= x >> 1 & 0x55555555;
            x = (x >> 2 & 0x33333333) + (x & 0x33333333);
            x = (x >> 4) + x & 0x0f0f0f0f;
            x += x >> 8;
            x += x >> 16;
            REGISTERS[RD] = bitset<32>(numIntBits - (x & 0x0000003f)).to_string();
            */
            temp_rs = REGISTERS[RS];
            count = 0;
            for (auto i = 0; i < temp_rs.length(); i++) {
                if (temp_rs[i] == '0')
                    count++;
                else
                    break;
            }
            temp_rd = bitset<32>(count).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case DIV:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            LO = sign_num(temp_rs) / sign_num(temp_rt);
            HI = sign_num(temp_rs) - LO * sign_num(temp_rt);
            return;
            break;
        case DIVU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            LO = stoi(temp_rs, nullptr, 2) - LO * stoi(temp_rt, nullptr, 2);
            return;
            break;
        case MULT:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_mult = sign_num(temp_rs) * sign_num(temp_rt);
            HI = (temp_mult & 0x1111111100000000) >> 32;
            LO = (temp_mult & 0x0000000011111111);
            return;
            break;
        case MULTU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_multu = stoi(temp_rt, nullptr, 2);
            HI = (temp_multu & 0x1111111100000000) >> 32;
            LO = (temp_multu & 0x0000000011111111);
            return;
            break;
        case MUL:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_mult = sign_num(temp_rs) * sign_num(temp_rt);
            temp_rd = bitset<32>(temp_mult & 0x0000000011111111).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case MADD:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_mult = sign_num(temp_rs) * sign_num(temp_rt);
            temp_mult += (HI << 32) | LO;
            HI = (temp_mult & 0x1111111100000000) >> 32;
            LO = (temp_mult & 0x0000000011111111);
            return;
            break;
        case MADDU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_multu = stoi(temp_rt, nullptr, 2);
            temp_multu += (HI << 32) | LO;
            HI = (temp_multu & 0x1111111100000000) >> 32;
            LO = (temp_multu & 0x0000000011111111);
            return;
            break;
        case MSUB:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_mult = sign_num(temp_rs) * sign_num(temp_rt);
            temp_mult -= (HI << 32) | LO;
            HI = (temp_mult & 0x1111111100000000) >> 32;
            LO = (temp_mult & 0x0000000011111111);
            return;
            break;
        case MSUBU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_multu = stoi(temp_rt, nullptr, 2);
            temp_multu -= (HI << 32) | LO;
            HI = (temp_multu & 0x1111111100000000) >> 32;
            LO = (temp_multu & 0x0000000011111111);
            return;
            break;
        case NOR:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(~(stoi(temp_rs, nullptr, 2) | stoi(temp_rt, nullptr, 2))).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case OR:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(stoi(temp_rs, nullptr, 2) | stoi(temp_rt, nullptr, 2)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case ORI:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = bitset<32>(stoi(temp_rs, nullptr, 2) | ins.etc).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case SLL:
            // RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            // temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(stoi(temp_rt, nullptr, 2) << ins.shamt).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SLLV:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(stoi(temp_rt, nullptr, 2) << stoi(temp_rs, nullptr, 2)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SRA:
            // no sense if the input number is not negative
            // RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            // temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(sign_num(temp_rt) >> ins.shamt).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SRAV:
            // no sense if the input number is not negative
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(sign_num(temp_rt) >> stoi(temp_rs, nullptr, 2)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SRL:
            // RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            // temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(stoi(temp_rt, nullptr, 2) >> ins.shamt).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SRLV:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(stoi(temp_rt, nullptr, 2) >> stoi(temp_rs, nullptr, 2)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SUB:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(sign_num(temp_rs) - sign_num(temp_rt)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SUBU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(stoi(temp_rs, nullptr, 2) - stoi(temp_rt, nullptr, 2)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case XOR:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            temp_rd = bitset<32>(stoi(temp_rs, nullptr, 2) ^ stoi(temp_rt, nullptr, 2)).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case XORI:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = bitset<32>(stoi(temp_rs, nullptr, 2) ^ ins.etc).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case LUI:
            RT = static_cast<REGISTER>(ins.rt);
            temp_rt = bitset<32>((ins.etc << 16) & 0x00000000).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case SLT:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (sign_num(temp_rs) < sign_num(temp_rt))
                temp_rd = bitset<32>(1).to_string();
            else
                temp_rd = bitset<32>(1).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SLTU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (stoi(temp_rt, nullptr, 2))
                temp_rd = bitset<32>(1).to_string();
            else
                temp_rd = bitset<32>(1).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case SLTI:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) < ins.etc)
                temp_rt = bitset<32>(1).to_string();
            else
                temp_rt = bitset<32>(0).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case SLTIU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            if (stoi(temp_rs, nullptr, 2) < ins.etc)
                temp_rt = bitset<32>(1).to_string();
            else
                temp_rt = bitset<32>(0).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case BEQ:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (temp_rs == temp_rt)
                PC = PC + ins.etc * 4;
            return;
            break;
        case BGEZ:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) >= 0)
                PC = PC + ins.etc * 4;
            return;
            break;
        case BGEZAL:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) >= 0) {
                PC = PC + ins.etc * 4;
                REGISTERS[static_cast<REGISTER>(31)] = bitset<32>(PC + 4).to_string();
            }
            return;
            break;
        case BGTZ:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) > 0)
                PC = PC + ins.etc * 4;
            return;
            break;
        case BLEZ:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) <= 0)
                PC = PC + ins.etc * 4;
            return;
            break;
        case BLTZAL:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) <= 0) {
                PC = PC + ins.etc * 4;
                REGISTERS[$ra] = bitset<32>(PC + 4).to_string();
            }
            return;
            break;
        case BLTZ:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) < 0)
                PC = PC + ins.etc * 4;
            return;
            break;
        case BNE:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (temp_rs != temp_rt)
                PC = PC + ins.etc * 4 + 4;
            return;
            break;
        case j:
            PC = (ins.etc << 2) | ((PC + 4) & 0xf0000000);
            return;
            break;
        case JAL:
            PC = (ins.etc << 2) | ((PC + 4) & 0xf0000000);
            REGISTERS[$ra] = bitset<32>(PC + 4).to_string();
            break;
        case JALR:
            RS = static_cast<REGISTER>(ins.rs);
            RD = static_cast<REGISTER>(ins.rd);
            temp_rs = REGISTERS[RS];
            PC = stoi(temp_rs, nullptr, 2);
            temp_rd = bitset<32>(PC + 4).to_string();
            REGISTERS[RD] = temp_rd;
            return;
            break;
        case JR:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            PC = stoi(temp_rs, nullptr, 2);
            return;
            break;
        case TEQ:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (temp_rs == temp_rt) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TEQI:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) == ins.etc) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TNE:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (temp_rs != temp_rt) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TNEI:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) != ins.etc) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TGE:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (sign_num(temp_rs) >= sign_num(temp_rt)) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TGEU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (stoi(temp_rs, nullptr, 2) >= stoi(temp_rt, nullptr, 2)) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TGEI:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) >= ins.etc) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TGEIU:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (stoi(temp_rs, nullptr, 2) >= ins.etc) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TLT:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (sign_num(temp_rs) < sign_num(temp_rt)) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TLTU:
            RS = static_cast<REGISTER>(ins.rs);
            RT = static_cast<REGISTER>(ins.rt);
            temp_rs = REGISTERS[RS];
            temp_rt = REGISTERS[RT];
            if (stoi(temp_rt, nullptr, 2)) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TLTI:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (sign_num(temp_rs) < ins.etc) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            return;
            break;
        case TLTIU:
            RS = static_cast<REGISTER>(ins.rs);
            temp_rs = REGISTERS[RS];
            if (stoi(temp_rs, nullptr, 2) < ins.etc) {
                cout << "A trap exception occurs." << endl;
                exit(EXIT_FAILURE);
            }
            break;
        case LB:
            RT = static_cast<REGISTER>(ins.rt);
            word_loc = ins.etc / 4 * 4;
            byte_loc = ins.etc - word_loc;
            temp_rt = bitset<32>(stoi(MEMORY[word_loc].substr(byte_loc * 8, 8), nullptr, 2) |
                                 0xffffff00).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case LBU:
            RT = static_cast<REGISTER>(ins.rt);
            word_loc = ins.etc / 4 * 4;
            byte_loc = ins.etc - word_loc;
            temp_rt = bitset<32>(stoi(MEMORY[word_loc].substr(byte_loc * 8, 8), nullptr, 2) |
                                 0x00000000).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case LH:
            RT = static_cast<REGISTER>(ins.rt);
            word_loc = ins.etc / 4 * 4;
            byte_loc = ins.etc - word_loc;
            temp_rt = bitset<32>(stoi(
                    MEMORY[word_loc].substr((byte_loc + 1) * 8, 8) +
                    MEMORY[word_loc].substr(byte_loc * 8, 8), nullptr, 2
            ) | 0xffff0000).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case LHU:
            RT = static_cast<REGISTER>(ins.rt);
            word_loc = ins.etc / 4 * 4;
            byte_loc = ins.etc - word_loc;
            temp_rt = bitset<32>(stoi(
                    MEMORY[word_loc].substr((byte_loc + 1) * 8, 8) +
                    MEMORY[word_loc].substr(byte_loc * 8, 8), nullptr, 2
            ) | 0x00000000).to_string();
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case LW:
            RT = static_cast<REGISTER>(ins.rt);
            word_loc = ins.etc;
            temp_rt = MEMORY[word_loc];
            REGISTERS[RT] = norm_to_little(temp_rt);
            return;
            break;
        case LWL:
            // MSB
            RT = static_cast<REGISTER>(ins.rt);
            word_loc = ins.etc / 4 * 4;
            byte_loc = ins.etc - word_loc;
            for (int i = 3; i >= byte_loc; i--) {
                temp_rt += MEMORY[word_loc].substr(i * 8, 8);
            }
            temp_rt += REGISTERS[RD].substr(temp_rt.length() - 1, byte_loc * 8);
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case LWR:
            // LSB
            RT = static_cast<REGISTER>(ins.rt);
            word_loc = ins.etc / 4 * 4;
            byte_loc = ins.etc - word_loc;
            for (int i = byte_loc; i >= 0; i--) {
                temp_rt += MEMORY[word_loc].substr(i * 8, 8);
            }
            temp_rt = REGISTERS[RD].substr(0, (3 - byte_loc) * 8) + temp_rt;
            REGISTERS[RT] = temp_rt;
            return;
            break;
        case LL:
            RT = static_cast<REGISTER>(ins.rt);
            word_loc = ins.etc;
            temp_rt = MEMORY[word_loc];
            REGISTERS[RT] = norm_to_little(temp_rt);
            return;
            break;
        case SB:
            RT = static_cast<REGISTER>(ins.rt);
            temp_rt = REGISTERS[RT].substr(24, 8);
            word_loc = ins.etc / 4 * 4;
            byte_loc = ins.etc - word_loc;
            for (int i = 0; i < 8; i++) {
                MEMORY[word_loc][byte_loc * 8 + i] = temp_rt[i];
            }
            return;
            break;
        case SH:
            RT = static_cast<REGISTER>(ins.rt);
            temp_rt = REGISTERS[RT].substr(16, 16);
            word_loc = ins.etc / 4 * 4;
            byte_loc = ins.etc - word_loc;
            for (int i = 8; i < 16; i++) {
                // REG: 0x00001234
                // MEM: 0x3412____ OR 0x____3412
                MEMORY[word_loc][byte_loc * 8 + i - 8] = temp_rt[i];
            }
            for (int i = 0; i < 8; i++) {
                MEMORY[word_loc][(byte_loc + 1) * 8 + i] = temp_rt[i];
            }
            return;
            break;
        case SW:
            RT = static_cast<REGISTER>(ins.rt);
            temp_rt = REGISTERS[RT];
            word_loc = ins.etc / 4 * 4;
            for (int i = 24; i < 32; i++) {
                MEMORY[word_loc][0 + i - 24] = temp_rt[i];
            }
            for (int i = 16; i < 24; i++) {
                MEMORY[word_loc][8 + i - 16] = temp_rt[i];
            }
            for (int i = 8; i < 16; i++) {
                // REG: 0x00001234
                // MEM: 0x3412____ OR 0x____3412
                MEMORY[word_loc][16 + i - 8] = temp_rt[i];
            }
            for (int i = 0; i < 8; i++) {
                MEMORY[word_loc][24 + i] = temp_rt[i];
            }
            return;
            break;
        case SWL:
            // not support
            cout << "SWL is not supported yet." << endl;
            exit(EXIT_FAILURE);
            break;
        case SWR:
            // not support
            cout << "SWR is not supported yet." << endl;
            exit(EXIT_FAILURE);
            break;
        case SC:
            RT = static_cast<REGISTER>(ins.rt);
            temp_rt = REGISTERS[RT];
            word_loc = ins.etc / 4 * 4;
            for (int i = 24; i < 32; i++) {
                MEMORY[word_loc][0 + i - 24] = temp_rt[i];
            }
            for (int i = 16; i < 24; i++) {
                MEMORY[word_loc][8 + i - 16] = temp_rt[i];
            }
            for (int i = 8; i < 16; i++) {
                // REG: 0x00001234
                // MEM: 0x3412____ OR 0x____3412
                MEMORY[word_loc][16 + i - 8] = temp_rt[i];
            }
            for (int i = 0; i < 8; i++) {
                MEMORY[word_loc][24 + i] = temp_rt[i];
            }
            REGISTERS[RT] = bitset<32>(1).to_string();
            return;
            break;
        case MFHI:
            RD = static_cast<REGISTER>(ins.rd);
            REGISTERS[RD] = bitset<32>(HI).to_string();
            return;
            break;
        case MFLO:
            RD = static_cast<REGISTER>(ins.rd);
            REGISTERS[RD] = bitset<32>(LO).to_string();
            return;
            break;
        case MTHI:
            RS = static_cast<REGISTER>(ins.rs);
            HI = stoi(REGISTERS[RS], nullptr, 2);
            return;
            break;
        case MTLO:
            RS = static_cast<REGISTER>(ins.rs);
            LO = stoi(REGISTERS[RS], nullptr, 2);
            return;
            break;
        case SYSCALL:
            syscall();
            break;
    };
}