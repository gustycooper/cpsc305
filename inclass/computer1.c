#include <stdio.h>

/*
C code
int a = 4, b = 4, c = 0;
c = a + b;

Assembly code

ldr r1, a
ldr r2, b
add r3, r1, r2
str r3, c

Each instruction is 32-bits, where specific bit positions are opcode, regs, and address.
There are three opcodes
0x01 - ldr
0x02 - str
0x03 - add
There are 16 registers - 0x0 through 0xf

The ldr and str instructions have the following format
  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 
  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |    opcode     |  reg  |                address                |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

The add instruction has the following format
  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 
  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |    opcode     |  reg  |  r1   |  r2   |                       |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Memory Layout - code at addr 0, data at addr 0x100
0x000 0x01100100    // ldr r1, a
0x004 0x01200104    // ldr r1, b
0x008 0x03312000    // add r3, r1, r2
0x00c 0x02300108    // str r3, c
...
0x100 0x00000004    // a = 4;
0x104 0x00000005    // b = 4;
0x108 0x00000000    // c = 0;
 */

// Define instruction opcodes
#define LDR_OPCODE 1
#define STR_OPCODE 2
#define ADD_OPCODE 3
// Define array to print mnemonics of opocdes
char *insts[] = {0, "ldr", "str", "add"};

// memory is a sequence of bytes - CPU is Little Endian
// When fetching bytes, they are composed into a 32-bit quantity
char memory[1024] = {0x00, 0x01, 0x10, 0x01, 0x04, 0x01, 0x20, 0x01, 0x00, 0x20, 0x31, 0x03, 0x08, 0x01, 0x30, 0x02};
// CPU has 16 registers. Each is 32-bits.
int registers[16] = { 0 };

// PROGRAM_SIZE is the number of instructions
#define PROGRAM_SIZE 4

int main(int argc, char *argv[]) {
    // a, b, and c are 4-byte ints, but their values fit into the first byte - other 3 are zero
    memory[0x100] = 4; // a = 4;
    memory[0x104] = 5; // b = 5;
    memory[0x108] = 0; // c = 0
    printf(" addr  :  contents  :  translation\n");
    for (int pc = 0; pc < PROGRAM_SIZE*4; pc+=4) {
        printf("0x%04x : ", pc);
// Fetch - create 32-bit instr from four bytes
        int inst = memory[pc] | (memory[pc+1]<<8) | (memory[pc+2]<<16) | (memory[pc+3]<<24);
        printf("0x%08x : ", inst);    // print hex of instr
// Decode - extract opcode, regs, and address from instr
        int opcode = inst >> 24;
        int rd = inst >> 20 & 0xf;
        int rm = inst >> 16 & 0xf;
        int rn = inst >> 12 & 0xf;
        int addr = inst & 0xfffff;
        printf("%s ", insts[opcode]); // print instruction opcode
        printf("r%d, ", rd);          // print dest reg
// Execute - the if statements decode the instruction (ldr, str, add)
        if (opcode == LDR_OPCODE || opcode == STR_OPCODE) {
            printf("0x%05x", addr);   // print address
            if (opcode == LDR_OPCODE) // execute ldr
                registers[rd] = memory[addr] | (memory[addr+1]<<8) | (memory[addr+2]<<16) | (memory[addr+3]<<24);
            else {                    // execute str
                memory[addr]   = registers[rd]       & 0xff;
                memory[addr+1] = registers[rd] >>  8 & 0xff;
                memory[addr+2] = registers[rd] >> 16 & 0xff;
                memory[addr+3] = registers[rd] >> 24 & 0xff;
            }
        }
        else {                        // ADD_OPCODE
            printf("r%d, ", rm);      // print first reg of add
            printf("r%d", rn);        // print second reg of add
            registers[rd] = registers[rm] + registers[rn]; // execute add
        }
        printf("\n");
    }
    // Note a, b, and c values fit into one byte so I just print the first byte
    printf("a: %d\n", memory[0x100]); // print value of a
    printf("b: %d\n", memory[0x104]); // print value of b
    printf("c: %d\n", memory[0x108]); // print value of c
}
