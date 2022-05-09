/*
 Charm opcodes are 8 bits. 
 The top 4 bits determine a category (e.g., load/store, arithmetic/logc)
 The bottop 4 bits determine instruction attributes

 This file is in both chasm and chemu.
 chasm does not use the upper 4 bits much because it has instruction category.
 */

enum charmopcode {
// Upper 4 bits of load/store opcodes
    LDR = 0x1,           // load reg with 4 bytes from memory
    LDB = 0x2,           // load reg with 1 byte from memory
    STR = 0x3,           // store reg into 4 bytes of memory
    STB = 0x4,           // store bottom 8 bits of reg in 1 byte of memory
// Lower 4 bits of load/store opcodes
    ADDR = 0x0,          // load/store addr
    BASE = 0x1,          // load/store [r]
    BASE_OFF = 0x2,      // load/store [r, imm]
    BASE_REG = 0x3,      // load/store [r, imm]
    PREINC_OFF = 0x4,    // load/store [r,imm]!
    PREINC_REG = 0x5,    // load/store [r,r]!
    POSTINC_OFF = 0x6,   // load/store [r],r
    POSTINC_REG = 0x7,   // load/store [r],r
// Upper 4 bits of arithmetic/logic opcodes
    ADD_RD_RM_RN = 0x5,  // inst is add r3, r4, r5
    ADD_RD_RM_IMM = 0x6, // inst is add r3, r4, 5
// Opcodes for arithmetic/logic for add rd, rm, rn  start at 0x50
// Opcoded for arithmetic/logic for add rd, rm, imm start at 0x60
// Bottom 4 bits arithmetic/logic instructions
    ADD = 0x0,
    SUB = 0x1,
    MUL = 0x2,
    DIV = 0x3,
    MOD = 0x4,
    AND = 0x5,
    ORR = 0x6,
    EOR = 0x7,
    ADC = 0x8,
    SBC = 0x9,
    ADF = 0xa,
    SUF = 0xb,
    MUF = 0xc,
    DIF = 0xd,
// Upper 4 bits of mov/cmp/shift instructions 
    MOV_RD_RM = 0x7,
    MOV_RD_IMM = 0x8,
// Bottom 4 bits mov/cmp/shft instructions
    MOV = 0x0,
    MVA = 0x1,
    CMP = 0x2,
    TST = 0x3,
    TEQ = 0x4,
    SHF = 0x5,
    SHA = 0x6,
    ROT = 0x7,
    ONE = 0x8,
    FTI = 0x9,
    ITF = 0xa,
    CMF = 0xb,
// Upper 4 bits of branch instructions determine branch destination
    B_ADDR = 0x9, // destination is an address
    B_REG = 0xa,  // destination is an address in an instruction
    B_REL = 0xb,  // destination is relative to pc
// Bottom 4 bits of branch instructions determine type of branch
    BAL = 0x0,    // branch always
    BEQ = 0x1,    // branch on equal
    BNE = 0x2,    // branch on not equal
    BLT = 0x3,    // branch on less than
    BLE = 0x4,    // branch on less than or equal
    BGT = 0x5,    // branch on greater than
    BGE = 0x6,    // branch on greater than or equal
    BLR = 0x7,    // branch on link register - function call
// Upper 4 bits of kernel instructions
    K_INST = 0xc, // destination is an address
// Bottom 4 bits of kernel instructions determin type of instruction
    KER = 0x0,   // user mode to kernel mode
    SRG = 0x1,   // update status register
    IOI = 0x2    // IO instructions
};
