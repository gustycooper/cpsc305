#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cpu.h"
#include "isa.h"

/*
 * Charm instructions have an 8-bit opcode field, which yields 256 instructions
 * The array insts has a non-zero value for each valid charm instuction opcode
 * Currenlty, the non-zero value is the addres of a string representing the instruction
 * This may be changed to a simple char insts[] instead of a char *insts[]
 */
char *insts[] = { 
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // opcodes 0-15 not used
/* 0x10 */  "ldr rd, address",
/* 0x11 */  "ldr rd, [rm]",
/* 0x12 */  "ldr rd, [rm, #imm]",
/* 0x13 */  "ldr rd, [rm, rm]",
/* 0x14 */  "ldr rd, [rm, #imm]!",
/* 0x15 */  "ldr rd, [rm, rm]!",
/* 0x16 */  "ldr rd, [rm], #imm",
/* 0x17 */  "ldr rd, [rm], rn",
            0,0,0,0,0,0,0,0, // opcodes 0x18-0x1f not used
/* 0x20 */  "ldb rd, address",
/* 0x21 */  "ldb rd, [rm]",
/* 0x22 */  "ldb rd, [rm, #imm]",
/* 0x23 */  "ldb rd, [rm, rm]",
/* 0x24 */  "ldb rd, [rm, #imm]!",
/* 0x25 */  "ldb rd, [rm, rm]!",
/* 0x26 */  "ldb rd, [rm], #imm",
/* 0x27 */  "ldb rd, [rm], rn",
            0,0,0,0,0,0,0,0, // opcodes 0x28-0x2f not used
/* 0x30 */  "str rd, address",
/* 0x31 */  "str rd, [rm]",
/* 0x32 */  "str rd, [rm, #imm]",
/* 0x33 */  "str rd, [rm, rm]",
/* 0x34 */  "str rd, [rm, #imm]!",
/* 0x35 */  "str rd, [rm, rm]!",
/* 0x36 */  "str rd, [rm], #imm",
/* 0x37 */  "str rd, [rm], rn",
            0,0,0,0,0,0,0,0, // opcodes 0x38-0x3f not used
/* 0x40 */  "stb rd, address",
/* 0x41 */  "stb rd, [rm]",
/* 0x42 */  "stb rd, [rm, #imm]",
/* 0x43 */  "stb rd, [rm, rm]",
/* 0x44 */  "stb rd, [rm, #imm]!",
/* 0x45 */  "stb rd, [rm, rm]!",
/* 0x46 */  "stb rd, [rm], #imm",
/* 0x47 */  "stb rd, [rm], rn",
            0,0,0,0,0,0,0,0, // opcodes 0x48-0x4f not used
/* 0x50 */  "add rd, rm, rn",
/* 0x51 */  "sub rd, rm, rn",
/* 0x52 */  "mul rd, rm, rn",
/* 0x53 */  "div rd, rm, rn",
/* 0x54 */  "and rd, rm, rn",
/* 0x55 */  "orr rd, rm, rn",
/* 0x56 */  "eor rd, rm, rn",
/* 0x57 */  "adc rd, rm, rn",
/* 0x58 */  "sbc rd, rm, rn",
/* 0x59 */  "adf rd, rm, rn",
/* 0x5a */  "sbf rd, rm, rn",
/* 0x5b */  "muf rd, rm, rn",
/* 0x5c */  "dif rd, rm, rn",
/* 0x5d */  "adi rd, rm, #imm",
/* 0x5e */  "sbi rd, rm, #imm",
            0, // opcodes 0x5d-0x5f not used
/* 0x60 */  "mov rd, rm",
/* 0x61 */  "mva rd, rm",
/* 0x62 */  "cmp rd, rm",
/* 0x63 */  "tst rd, rm",
/* 0x64 */  "teq rd, rm",
/* 0x65 */  "shf rd, rm",
/* 0x66 */  "sha rd, rm",
/* 0x67 */  "rot rd, rm",
            0,0,0,0,0,0,0,0, // opcodes 0x68-0x6f not used
/* 0x70 */  "mov rd, #imm",
/* 0x71 */  "mva rd, #imm",
/* 0x72 */  "cmp rd, #imm",
/* 0x73 */  "tst rd, #imm",
/* 0x74 */  "teq rd, #imm",
/* 0x75 */  "shf rd, #imm",
/* 0x76 */  "sha rd, #imm",
/* 0x77 */  "rot rd, #imm",
            0,0,0,0,0,0,0,0, // opcodes 0x78-0x7f not used
/* 0x80 */  "bal address",
/* 0x81 */  "beq address",
/* 0x82 */  "bne address",
/* 0x83 */  "blt address",
/* 0x84 */  "ble address",
/* 0x85 */  "bgt address",
/* 0x86 */  "bge address",
/* 0x87 */  "blr address",
            0,0,0,0,0,0,0,0, // opcodes 0x88-0x8f not used
/* 0x90 */  "bal [rd]",
/* 0x91 */  "beq [rd]",
/* 0x92 */  "bne [rd]",
/* 0x93 */  "blt [rd]",
/* 0x94 */  "ble [rd]",
/* 0x95 */  "bgt [rd]",
/* 0x96 */  "bge [rd]",
/* 0x97 */  "blr [rd]",
            0,0,0,0,0,0,0,0, // opcodes 0x98-0x9f not used
/* 0xa0 */  "bal off",
/* 0xa1 */  "beq off",
/* 0xa2 */  "bne off",
/* 0xa3 */  "blt off",
/* 0xa4 */  "ble off",
/* 0xa5 */  "bgt off",
/* 0xa6 */  "bge off",
/* 0xa7 */  "blr off",
            0,0,0,0,0,0,0,0, // opcodes 0xa8-0xaf not used
/* 0xb0 */  "ker #imm",
/* 0xb1 */  "srg #imm",
/* 0xb2 */  "ioi #imm",
            0,0,0,0,0,0,0,0,0,0,0,0,0,       // opcodes 0xb3-bf not used
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // opcodes 0xc0-cf not used
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // opcodes 0xd0-df not used
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // opcodes 0xe0-ef not used
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  // opcodes 0xf0-ff not used
};


static char *ldrstr_strs[] = { "ldr ", "ldb ", "str ", "stb " };
static char *arilog_strs[] = { "add ", "sub ", "mul ", "div ", "and ", "orr ", "eor ", "adc ", "sbc ", "adf ", "sbf ", "muf ", "dif ", "adi ", "sbi " };
static char *movcmp_strs[] = { "mov ", "mva ", "cmp ", "tst ", "teq ", "shf ", "sha ", "rot " };
static char *branch_strs[] = { "bal ", "beq ", "bne ", "blt ", "ble ", "bgt ", "bge ", "blr " };
static char *kernel_strs[] = { "ker ", "srg ", "ioi " };
static char disassembled[100]; // holds a disassembled instruction

/*
 * Disassembles inst into the char array disassembled
 * Returns the address of the char array disassembled
 * Calling functions must duplicate the returned string if needed prior to subsequent calls
 */
char *disassemble(unsigned int inst) {
    decoded *d = decode(inst);
    disassembled[0] = 0;
    char *p = disassembled;
    if (d == 0) {
        p = strcpy(p, "not instruction");
        return disassembled;
    }

    char buf[25];
    switch (d->opcode >> 4 & 0xf) {
      case 1: case 2: case 3: case 4: // ldr, ldb, str, stb
        p = strcat(p, ldrstr_strs[(d->opcode >> 4 & 0xf)-1]);
        sprintf(buf, "r%d, ", d->rd);
        p = strcat(p, buf);
        switch (d->opcode & 0xf) {
          case 0:
            sprintf(buf, "0x%x", d->address);
            break;
          case 1:
            sprintf(buf, "[r%d]", d->rm);
            break;
          case 2:
            sprintf(buf, "[r%d, #%d]", d->rm, d->immediate16);
            break;
          case 3:
            sprintf(buf, "[r%d, r%d]", d->rm, d->rn);
            break;
          case 4:
            sprintf(buf, "[r%d, #%d]!", d->rm, d->immediate16);
            break;
          case 5:
            sprintf(buf, "[r%d, r%d]!", d->rm, d->rn);
            break;
          case 6:
            sprintf(buf, "[r%d], #%d", d->rm, d->immediate16);
            break;
          case 7:
            sprintf(buf, "[r%d], r%d", d->rm, d->rn);
            break;
          default:
            sprintf(buf, "bad ldrstr");
            
        }
        p = strcat(p, buf);
        break;
      case 5: // arilog inst
        p = strcat(p, arilog_strs[d->opcode & 0xf]);
        if (d->opcode >= 0x50 && d->opcode <= 0x5c)
            sprintf(buf, "r%d, r%d, r%d", d->rd, d->rm, d->rn);
        else
            sprintf(buf, "r%d, r%d, #%d", d->rd, d->rm, d->immediate16);
        p = strcat(p, buf);
        break;
      case 6: case 7: // movcmp inst
        p = strcat(p, movcmp_strs[d->opcode & 0xf]);
        if ((d->opcode >> 4 & 0xf) == 6) // rd, rm
            sprintf(buf, "r%d, r%d", d->rd, d->rm);
        else // rd, #imm
            sprintf(buf, "r%d, #%d", d->rd, d->immediate16);
        p = strcat(p, buf);
        break;
      case 8: case 9: case 10: // branch inst
        p = strcat(p, branch_strs[d->opcode & 0xf]);
        if ((d->opcode >> 4 & 0xf) == 8) // bal address
            sprintf(buf, "#%08x", d->address);
        else if ((d->opcode >> 4 & 0xf) == 7) // bal [rd]
            sprintf(buf, "[r%d]", d->rd);
        else // bal offset
            sprintf(buf, "#%d", d->immediate20);
        p = strcat(p, buf);
        break;
      case 11: // ker innstruction
        p = strcat(p, kernel_strs[d->opcode & 0xf]);
        sprintf(buf, "#%08x", d->address);
        p = strcat(p, buf);
        break;
    }
    free(d);
    return disassembled;
}


/*
 * Decodes inst, placing instruction fields into members of a decoded struct
 * mallocs memory for the returned value
 * The instruction type is not examined. Simply extracts fields. 
 * Calling function must examine instruction type and conditionally use returned decoded members
 * Calling function is responsible for freeing memory when done
 */
decoded *decode(unsigned int inst) {
    decoded *d = malloc(sizeof(decoded));
    d->opcode = inst >> 24;
    d->rd = inst >> 20 & 0xf;
    d->rm = inst >> 16 & 0xf;
    d->rn = inst >> 12 & 0xf;
    d->address = inst & 0xfffff;
    int a = inst & 0xfffff;
    a <<= 12;  // convert to signed 32-bit value, shift left by 8
    a /= 4096; // shift right by 8 and propagate sign bit (>> may be logical)
    d->immediate20 = a;
    a = inst & 0xffff;
    a <<= 16;   // convert to signed 32-bit value, shift left by 16
    a /= 65536; // shift right by 16 and propagate sign bit (>> may be logical)
    d->immediate16 = a;
    d->offset = inst >> 8 & 0xff;

    if (insts[d->opcode] == 0) {
        free(d);
        return 0;
    }
    else
        return d;
}
