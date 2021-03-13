# mips-assembler-simulator

## Project Overview

This project is the first assignment of CSC3050 Computer Architecture in LGU. A simplified MIPS assembler and a corresponding simulator are coded.

## Simplified MIPS assembler
This assembler does not support pseudo-instructions nor co-processor instructions. It strictly follows the format in the textbook.

The supporting instructions are listed below:
```
    ADD,         // r, op: 00, funct: 20
    ADDU,        // r, op: 00, funct: 21
    ADDI,        // i, op: 08
    ADDIU,       // r, op: 09
    AND,         // r, op: 00, funct: 24
    ANDI,        // r, op: 0c
    CLO,         // r, op: 1c, funct: 21
    CLZ,         // r, op: 1c, funct: 20
    DIV,         // r, op: 00, funct: 1a
    DIVU,        // r, op: 00, funct: 1b
    MULT,        // r, op: 00, funct: 18
    MULTU,       // r, op: 00, funct: 19
    MUL,         // r, op: 1c, funct: 02
    MADD,        // r, op: 1c, funct: 00
    MADDU,       // r, op: 1c, funct: 01
    MSUB,        // r, op: 1c, funct: 04
    MSUBU,       // r, op: 1c, funct: 05
    NOR,         // r, op: 00, funct: 27
    OR,          // r, op: 00, funct: 25
    ORI,         // i, op: 0d
    SLL,         // r, op: 00, funct: 00
    SLLV,        // r, op: 00, funct: 04
    SRA,         // r, op: 00, funct: 03
    SRAV,        // r, op: 00, funct: 37
    SRL,         // r, op: 00, funct: 02
    SRLV,        // r, op: 00, funct: 06
    SUB,         // r, op: 00, funct: 22
    SUBU,        // r, op: 00, funct: 23
    XOR,         // r, op: 00, funct: 26
    XORI,        // i, op: 0e
    LUI,         // i, op: 0f
    SLT,         // r, op: 00, funct: 2a
    SLTU,        // r, op: 00, funct: 2b
    SLTI,        // i, op: 0a
    SLTIU,       // i, op: 0b
    BEQ,         // i, op: 04
    BGEZ,        // i, op: 01 *
    BGEZAL,      // i, op: 01 *
    BGTZ,        // i, op: 07
    BLEZ,        // i, op: 06
    BLTZAL,      // i, op: 01 *
    BLTZ,        // i, op: 01 *
    BNE,         // i, op: 05
    J,           // j, op: 02
    JAL,         // g, op: 03
    JALR,        // r, op: 00, funct: 09
    JR,          // r, op: 00, funct: 08
    TEQ,         // r, op: 00, funct: 34
    TEQI,        // i, op: 01 *
    TNE,         // r, op: 00, funct: 36
    TNEI,        // i, op: 01 *
    TGE,         // r, op: 00, funct: 30
    TGEU,        // r, op: 00, funct: 31
    TGEI,        // i, op: 01 *
    TGEIU,       // i, op: 01 *
    TLT,         // r, op: 00, funct: 32
    TLTU,        // r, op: 00, funct: 33
    TLTI,        // i, op: 01 *
    TLTIU,       // i, op: 01 *
    LB,          // i, op: 20
    LBU,         // i, op: 24
    LH,          // i, op: 21
    LHU,         // i, op: 25
    LW,          // i, op: 23
    LWL,         // i, op: 22
    LWR,         // i, op: 26
    LL,          // i, op: 30
    SB,          // i, op: 28
    SH,          // i, op: 29
    SW,          // i, op: 2b
    SWL,         // i, op: 2a
    SWR,         // i, op: 2e
    SC,          // i, op: 38
    MFHI,        // r, op: 00, funct: 10
    MFLO,        // r, op: 00, funct: 12
    MTHI,        // r, op: 00, funct: 11
    MTLO,        // r, op: 00, funct: 13
    SYSCALL      // r, op: 00, funct: 0c
```



