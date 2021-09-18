#include <stdio.h>

/*
C code
int a = 4, b = 4, z = 0;
z = a + b;

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
0x108 0x00000000    // z = 0;
 */

char *insts[] = {0, "ldr", "str", "add"};

// memory should be char, but int simplifies fetching
// When fetching bytes, they must be composed into a 32-bit quantity
int memory[1024] = {0x01100100, 0x01200104, 0x03312000, 0x02300108};
int registers[16] = { 0 };

#define PROGRAM_SIZE 4

int main(int argc, char *argv[]) {
    memory[0x100] = 4; // x = 4;
    memory[0x104] = 5; // y = 5;
    memory[0x108] = 0; // z = 0
    printf(" addr  :  contents  :  translation\n");
    for (int pc = 0; pc < PROGRAM_SIZE; pc++) {
        printf("0x%04x : ", pc*4);
// Fetch
        int inst = memory[pc];
        printf("0x%08x : ", inst); // print hex of instr
// Decode
        int opcode = inst >> 24;
        int rd = inst >> 20 & 0xf;
        int rm = inst >> 16 & 0xf;
        int rn = inst >> 12 & 0xf;
        int addr = inst & 0xfffff;
        printf("%s ", insts[opcode]); // print instruct
        printf("r%d, ", rd);          // print dest reg
// Execute
        if (opcode < 3) {
            printf("0x%05x", addr);   // print address
            if (opcode == 1)
                registers[rd] = memory[addr]; // execute ldr
            else
                memory[addr] = registers[rd]; // execute str
        }
        else {
            printf("r%d, ", rm);      // print first reg of add
            printf("r%d", rn);        // print second reg of add
            registers[rd] = registers[rm] + registers[rn]; // execute add
        }
        printf("\n");
    }
    printf("a: %d\n", memory[0x100]); // print value of a
    printf("b: %d\n", memory[0x104]); // print value of b
    printf("c: %d\n", memory[0x108]); // print value of c
}
