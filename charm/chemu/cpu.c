#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "memory.h"
#include "bit_functions.h"
#include "cpu.h"
#include "isa.h"

extern char memory[];
void addresult(char *res);

/******************************************************************
 ***************************  EXECUTE  ****************************
 ** global verbose_cpu - main() changes this via v command       **
 ******************************************************************/
int verbose_cpu = 0;

/******************************************************************
 ************************  INST HISTORY  **************************
 ** save the last INSTHIST instructions executed                 **
 ******************************************************************/
#define INSTHIST 5
struct insthist {
    int addr;
    int inst;
};
static int insthist_i = 0; // next available slot in insthist
static struct insthist insthist[INSTHIST] = { {0,0} };

/******************************************************************
 **************************  REGISTERS  ***************************
 ******************************************************************/
int registers[16]; // 16 32-bit registers
int cpsr;          // status register
int rupt;          // base address of interrupt vector table
                   // *(rupt+0) - address of user to kernel interrupt
                   // *(rupt+4) - address of I/O interrupt

/******************************************************************
 *************************  PIPELINE  *****************************
 ** display last 5 inst, current inst, and next 5 inst           **
 ******************************************************************/
void addinst(char *inst);
char instresult[80]; // used to add a result to ncurses result window

void pipeline() {
    int x = insthist_i;
    for (int i = 0; i < INSTHIST; i++) {
        if (verbose_cpu)
            sprintf(instresult, "0x%08x: 0x%08x: %s", insthist[x].addr, insthist[x].inst, disassemble(insthist[x].inst));
        else
            sprintf(instresult, "0x%08x: %s", insthist[x].addr, disassemble(insthist[x].inst));
        addinst(instresult);
        x = (x + 1) % INSTHIST;
    }
    int address = registers[PC], inst, branch = 0, baddress;
    for (int i = 0; i < INSTHIST+1; i++) {
        system_bus(address, &inst, READ);
        if (verbose_cpu)
            sprintf(instresult, "0x%08x: 0x%08x: %s", address, inst, disassemble(inst));
        else
            sprintf(instresult, "0x%08x: %s", address, disassemble(inst));
        if (branch) { // branch displays both paths of branch
            char branresult[128];
            system_bus(baddress, &inst, READ);
            if (verbose_cpu)
                sprintf(branresult, "%s  0x%08x: 0x%08x: %s", instresult, baddress, inst, disassemble(inst));
            else
                sprintf(branresult, "%s  0x%08x: %s", instresult, baddress, disassemble(inst));
            strncpy(instresult, branresult, 80);
            addinst(instresult);
            baddress += 4;
        }
        if (i == 0) {
            strcat(instresult, "<- pc");
            //printf(" <- pc");
            int opcodeupper = (inst >> 28) & 0xf; // TODO - should use decode() for this
            if (opcodeupper >= 8 && opcodeupper <= 10) {
                branch = 1;
                baddress = inst & 0xfffff; // TODO - get address from reg, etc.
            }
        }
        addinst(instresult);
        address += 4;
    }
}

/******************************************************************
 ***********************  SETTERS/GETTERS *************************
 ******************************************************************/
void set_reg(int reg, int value) {
    registers[reg] =  value;
}

int get_reg(int reg) {
    return registers[reg];
}

void set_cpsr(int bit) {
    bit_set(&cpsr, bit);
}

int get_cpsr() {
    return cpsr;
}

void set_rupt(int value) {
    rupt =  value;
}

int get_rupt() {
    return rupt;
}

/******************************************************************
 ***********************  CHEMU IOI *******************************
 ******************************************************************
 *
 * User Mode initiates a printf via ker 0x11 and scanf via ker 0x10
 * The 0x10 and 0x11 are passed to kernel in r0.
 * Prior to issuing the ker 0x10/0x11, a user places a fmt string in
 * r1. For each %fmt in the fmt string, the uses places a matching
 * parameter in r2 and r3.
 * The ker instruction traps to the OS. The OS address is placed in the
 * interrupt vector table, which is located at address 0x7ff0.
 * The OS issues a kernel mode ioi insruction to perform the scanf/printf.
 * The ioi operand is 0x10 for scanf and 0x11 for printf.
 * Register values when chemuioi occurs are the following.
 * r0 - contains 0x10 or 0x11
 * r1 - contains the format string
 * r2 - conaints value to match first %fmt
 * r3 - contains value to match second %fmt
 */
void byteswap(int *p) {
    char b0 = *p         & 0xff;
    char b1 = (*p >> 8)  & 0xff;
    char b2 = (*p >> 16) & 0xff;
    char b3 = (*p >> 24) & 0xff;
    *p = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}
int doscanf = 0, scanfdone = 0;
static int perc; // the number of % in fmt string
// chemu is big endian, laptops are little endian
// After scanf(%d), must byteswap. percd[] tracks %d
static int percd[2]; // a 1 indicates %d
// cheumscanf called from main() in chemu.c to apply scanf to str.
// char *str points the the user's input
void chemuscanf(char *str) {
    //char temp[80];
    //snprintf(temp, 80, "%s %d %d\n", str, perc, percd[0]);
    //addresult(temp);
    if (perc == 1) {
        sscanf(str, &memory[registers[1]], &memory[registers[2]]);
        if (percd[0]) // must byteswap
            byteswap((int *)&memory[registers[2]]);
    }
    else if (perc == 2) {
        sscanf(str, &memory[registers[1]], &memory[registers[2]], &memory[registers[3]]);
        if (percd[0]) // must byteswap
            byteswap((int *)&memory[registers[2]]);
        if (percd[1]) // must byteswap
            byteswap((int *)&memory[registers[3]]);
    }
    doscanf = 0;
    scanfdone = 1;

}

void chemuioi(int op2) {
    percd[0] = 0;
    percd[1] = 0;
    if (op2 == 0x10 || op2 == 0x11) { // scanf or printf
        char *p = &memory[registers[1]];
        perc = 0; // count % chars in format string
        for (int i = 0; i < 80; i++) {
            if (*p == 0)
                break;
            if (*p++ == '%') {
                perc++;
                if (*p == 'd') // %d
                    percd[perc-1] = 1;
            }
        }
        if (perc > 2)
            perc = 2; // max of two format chars
        char temp[80];
        if (op2 == 0x11) { // printf
            switch (perc) {
              case 0:
                strncpy(temp, &memory[registers[1]], 80);
                break;
              case 1:
                snprintf(temp, 80, &memory[registers[1]], registers[2]);
                break;
              case 2:
                snprintf(temp, 80, &memory[registers[1]], registers[2], registers[3]);
                break;
            }
            addresult(temp);
        }
        else { // scanf
            doscanf = 1;
        }
   }
}

/******************************************************************
 ************************  BREAKPOINT *****************************
 ******************************************************************/
// TODO - add more breakpoints #define NUM_BREAKPOINTS 2
// static int breakpoints[NUM_BREAKPOINTS]; // allow 2 break points

// global breakpoint - main() changes this via b command
int breakpoint = -1; // >0 is address of a breakpoint
static int breakpointexecuted = 0;
static int memaddr_changed = 0, memval_before = 0, memval_after = 0;

/******************************************************************
 ****************************  STEP  ******************************
 ** step() does fetch, decode, execute for one instruction       **
 ** return 0 normal                                              **
 ** return 1 when breakpoint is hit                              **
 ** return 2 when waiting on user input for scanf                **
 ** return -1 for illegal instruction                            **
 ******************************************************************/
int step() {
    memaddr_changed = 0; // state of changed memory
/******************************************************************
 ****************************  FETCH  *****************************
 ******************************************************************/
    int pc = registers[PC];
    if (pc == breakpoint && !breakpointexecuted) {
        breakpointexecuted = 1;
        return 1;
    }
    breakpointexecuted = 0;
    int inst; 
    system_bus(pc, &inst, READ);
    insthist[insthist_i].addr = pc;
    insthist[insthist_i].inst = inst;
    insthist_i = (insthist_i + 1) % INSTHIST;
/******************************************************************
 ****************************  DECODE  ****************************
 ******************************************************************/
    decoded *d = decode(inst);
    if (!d) {
        printf("Invalid instruction at address: %08x", registers[PC]);
        printf("pc: 0x%08x, inst: 0x%08x\n", registers[PC], inst);
        registers[PC] = pc;
        return -1;

    }
    if (verbose_cpu)
        printf("pre-step  : cpsr: 0x%08x, pc: 0x%08x, inst: 0x%08x, %s\n", cpsr, registers[PC], inst, disassemble(inst));
    int address = 0, control;
    int retval = 0; // normal return
    int opcode = d->opcode >> 4 & 0xf; // & 0xf needed due to arithmetic left shift
/******************************************************************
 ***************************  EXECUTE  ****************************
 ** Each case of the switch statements perform execute           **
 ******************************************************************/
    switch (opcode) {
      case 1: case 2: case 3: case 4: // ldr, ldb, str, stb
        if (opcode < 3)
            control = READ;
        else
            control = WRITE;
        int word = 1; // assume ldr and str, which are 32-bit operations
        if (opcode == 2 || opcode == 4) // 2 is ldb, 4 is stb
            word = 0; // ldb or stb
        switch (d->opcode & 0xf) {
          case 0:
            address = d->address;
            break;
          case 1:
            address = registers[d->rm];
            break;
          case 2:
            address = registers[d->rm] + d->immediate16;
            break;
          case 3:
            address = registers[d->rm] + registers[d->rn];
            break;
          case 4:
            registers[d->rm] += d->immediate16;
            address = registers[d->rm];
            break;
          case 5:
            registers[d->rm] += registers[d->rn];
            address = registers[d->rm];
            break;
          case 6:
            address = registers[d->rm];
            registers[d->rm] += d->immediate16;
            break;
          case 7:
            address = registers[d->rm];
            registers[d->rm] += registers[d->rn];
            break;
          default:
            printf("bad ldrstr\n");
            retval = -1;
        }
        unsigned char ldbstb;
        if (control == WRITE) {
            memaddr_changed = address;
            if (word) {
                memval_after = registers[d->rd];  // str changes 32-bits
                system_bus(address, &memval_before, READ);
            }
            else {
                memval_after = registers[d->rd] & 0xff; // stb changes a byte
                system_bus_b(address, &ldbstb, READ);
                memval_before = ldbstb;
            }
        }
        if (word)
            system_bus(address, &registers[d->rd], control);
        else {
            ldbstb = registers[d->rd] & 0xff;
            system_bus_b(address, &ldbstb, control);
            if (control == READ)
                registers[d->rd] = ldbstb;
        }
        pc += 4;
        break;
      case 5: // arilog inst
        switch (d->opcode & 0xf) {
          float frd, frm, frn;
          case 0:
              registers[d->rd] = registers[d->rm] + registers[d->rn];
              break;
          case 1:
              registers[d->rd] = registers[d->rm] - registers[d->rn];
              break;
          case 2:
              registers[d->rd] = registers[d->rm] * registers[d->rn];
              break;
          case 3:
              registers[d->rd] = registers[d->rm] / registers[d->rn];
              break;
          case 4:
              registers[d->rd] = registers[d->rm] & registers[d->rn];
              break;
          case 5:
              registers[d->rd] = registers[d->rm] | registers[d->rn];
              break;
          case 6:
              registers[d->rd] = registers[d->rm] ^ registers[d->rn];
              break;
          case 7: // TODO fix so it does add with carry
              registers[d->rd] = registers[d->rm] + registers[d->rn];
              break;
          case 8: // TODO fix so it does sub with carry
              registers[d->rd] = registers[d->rm] - registers[d->rn];
              break;
          case 9:
              frm = (float)registers[d->rm];
              frn = (float)registers[d->rn];
              frd = frm + frn;
              registers[d->rd] = (int)frd;
              break;
          case 10:
              frm = (float)registers[d->rm];
              frn = (float)registers[d->rn];
              frd = frm - frn;
              registers[d->rd] = (int)frd;
              break;
          case 11:
              frm = (float)registers[d->rm];
              frn = (float)registers[d->rn];
              frd = frm * frn;
              registers[d->rd] = (int)frd;
              break;
          case 12:
              frm = (float)registers[d->rm];
              frn = (float)registers[d->rn];
              frd = frm / frn;
              registers[d->rd] = (int)frd;
              break;
          case 13:
              registers[d->rd] = registers[d->rm] + d->immediate16;
              break;
          case 14:
              registers[d->rd] = registers[d->rm] - d->immediate16;
              break;
          default:
            printf("bad arilog\n");
            retval = -1;
        }
        pc += 4;
        break;
      case 6: case 7: // movcmp inst
        switch (d->opcode & 0xf) {
          case 0: case 1:  // mov mva
            if (opcode == 6) // mov rd, rm or mva rd, rm
                registers[d->rd] = registers[d->rm];
            else if (opcode == 7 && ((d->opcode & 0xf) == 1)) // mva rd, #addr
                registers[d->rd] = d->address;
            else // mov rd, #imm
                registers[d->rd] = d->immediate20;
            pc += 4;
            if (d->rd == 15) // mov r15, rXX
                pc = registers[PC];
            break;
          case 2: case 3: case 4:; // cmp, tst teq
            int val, op2;
            if (opcode == 6) // rd, rm
                op2 = registers[d->rm];
            else // rd, #imm
                op2 = d->immediate20;
            switch (d->opcode & 0xf) {
              case 2: // cmp
                val = registers[d->rd] - op2;
                break;
              case 3: // tst
                val = registers[d->rd] & op2;
                break;
              case 4: // teq
                val = registers[d->rd] ^ op2;
                break;
            }
            if (val == 0) {
                bit_set(&cpsr, Z);
                bit_clear(&cpsr, N);
                bit_clear(&cpsr, C);
                bit_clear(&cpsr, V);
            }
            else if (val < 0) {
                bit_clear(&cpsr, Z);
                bit_set(&cpsr, N);
                bit_clear(&cpsr, C);
                bit_clear(&cpsr, V);
            }
            else {
                bit_clear(&cpsr, Z);
                bit_clear(&cpsr, N);
                bit_clear(&cpsr, C);
                bit_clear(&cpsr, V);
            }
            pc += 4;
            break;
          case 5: case 6: case 7: // shf, sha, rot
            if (opcode == 6) // rd, rm
                op2 = registers[d->rm];
            else // rd, #imm
                op2 = d->immediate20;
            int left = 1;       // op2 > 0 is a left shift. reg value gets bigger
            if (op2 < 0) {  // op2 < 0 is a right shift. reg value gets smaller
                left = 0;
                op2 = -op2;
            }
            op2 %= 32; // make shift < 32
            switch (d->opcode & 0xf) {
              case 5: // shf - logical shift
                if (left)
                    registers[d->rd] = registers[d->rd] << op2;
                else
                    // >> may be arithmetic shift. C standard does not state the type of shift
                    registers[d->rd] = (int)((unsigned int)registers[d->rd] >> op2);
                break;
              case 6: // sha - arithmetic shift
                if (left) // left shift, propagate bit 0
                    if (bit_test(registers[d->rd], 0))
                        registers[d->rd] = ~(~registers[d->rd] << op2);
                    else
                        registers[d->rd] = registers[d->rd] << op2;
                else         // right shift, propagate bit 31
                             // reg < 0 ? ~(~reg >> op2) : reg >> op2;
                    if (bit_test(registers[d->rd], 31))
                        registers[d->rd] = ~(~registers[d->rd] >> op2);
                    else
                        registers[d->rd] = registers[d->rd] >> op2;
                break;
              case 7: // rot - rotate
                if (left)
                    registers[d->rd] = (registers[d->rd] << op2) | 
                                       (int)((unsigned int)registers[d->rd] >> (32 - op2));
                else
                    registers[d->rd] = (registers[d->rd] >> op2) | (registers[d->rd] << (32 - op2));
                break;
            }
            pc += 4;
            break;
        }
        break;
      case 8: case 9: case 10: // branch inst
        if (opcode == 8)
            address = d->address;
        else if (opcode == 9)
            address = registers[d->rd];
        else
            address = registers[PC] + d->immediate20;
        switch (d->opcode & 0xf) {
          case 0:
              pc = address;
              break;
          case 1:
              if (bit_test(cpsr, Z))
                  pc = address;
              else
                  pc += 4;
              break;
          case 2:
              if (!bit_test(cpsr, Z))
                  pc = address;
              else
                  pc += 4;
              break;
          case 4:
              if (bit_test(cpsr, Z) || (bit_test(cpsr, N) != bit_test(cpsr, V)))
                  pc = address;
              else
                  pc += 4;
              break;
          case 3:
              if ((bit_test(cpsr, N) != bit_test(cpsr, V)))
                  pc = address;
              else
                  pc += 4;
              break;
          case 6:
              if ((bit_test(cpsr, N) == bit_test(cpsr, V)))
                  pc = address;
              else
                  pc += 4;
              break;
          case 5:
              if (!bit_test(cpsr, Z) && (bit_test(cpsr, N) == bit_test(cpsr, V)))
                  pc = address;
              else
                  pc += 4;
              break;
          case 7:
              registers[LR] = pc + 4;
              pc = address;
              break;
          default:
              printf("Bad Branch Instruction\n");
              retval = -1;
        }
        break;
      case 11: // kernel inst
         switch (d->opcode & 0xf) {
           case 0: // 0xb0 - ker, user to kernel mode
               if (bit_test(cpsr, U) && bit_test(cpsr, OS)) { // user mode and OS loaded
                   registers[0] = d->immediate20;  // ker #imm value to r0
                   registers[LR] = pc + 4;         // return address to LR
                   bit_clear(&cpsr, U);            // clear user mode bit
                   system_bus(rupt, &pc, READ);    // get base addr of OS from interrupt table
               }
               else {
                   printf("Illegal use of ker Instruction\n");
                   retval = -1;
               }
               break;
           case 1: // 0xb1 - srg, status reg instruction
               if (!bit_test(cpsr, U)) { // 0xb1 must be done in kernel mode
                   int bit_pos = d->immediate20 & 0x1f; // 5 bits for 0 to 31
                   if (d->immediate20 >> 5 & 1)
                      bit_set(&cpsr, bit_pos);
                   else
                      bit_clear(&cpsr, bit_pos);
                   pc += 4;
               }
               else {
                   printf("Illegal use of srg Instruction\n");
                   retval = -1;
               }
               break;
           case 2: // 0xb2 - ioi, input output instruction
               // 0xb2 must be done in kernel mode with the OS loaded
               if (!bit_test(cpsr, U) && bit_test(cpsr, OS)) {
                   if (scanfdone) {
                       pc += 4;
                       scanfdone = 0;
                   }
                   else {
                       chemuioi(d->immediate20);
                       if (doscanf)
                           return 2;
                       else
                           pc += 4;
                   }
               }
               else {
                   printf("Illegal use of ioi Instruction\n");
                   retval = -1;
               }
               break;
         }
       break;
    }
    free(d); // free the decoded struct
    registers[PC] = pc;
    if (verbose_cpu) {
        system_bus(pc, &inst, READ);
        printf("post-step:  cpsr: 0x%08x, pc: 0x%08x, inst: 0x%08x, %s\n", cpsr, registers[PC], inst, disassemble(inst));
    }
    return retval;
}

// return values
//  0 steps all steps
//  1 breakpoint
//  2 bal to itself
//  3 waiting on input from scanf
/******************************************************************
 ***************************  STEP_N  *****************************
 ** step_n() calls step() n times                                **
 ** return values                                                **
 **  return -1 step_n encountered an illegal instruction         **
 **  return 0  step_n stepped all steps                          **
 **  return 1  step_n encountered a breakpoint                   **
 **  return 2  step_n encountered a bal to itself                **
 ******************************************************************/
int step_n(int n) {
    int s = 0;
    for (int i = 0; i < n; i++) {
        int pc = registers[15];
        s = step();
        if (s == 1) // 1 means a breakpoint is hit
            return 1;
        else if (s == 2) // 2 means waiting on input from scanf
            return 3;
        else if (s < 0)
            return -1;
        if (registers[15] == pc)
            return 2;
    }
    return s;
}

/******************************************************************
 *********************  SHOW_MEM_CHANGED  *************************
 ** the variables memaddr_changed, memval_before, memval_after   **
 ** are modified by step() and used by show_mem_changed()        **
 ** show_mem_changed() sprintf's the changed values into the     **
 ** variable mem_changed, which is displayed by chemu.c          **
 ******************************************************************/
// mem_changed is written to regisers window
char mem_changed[80] = "addr: 0xffff, before: 0xffffffff, after: 0xffffffff";
void show_mem_changed() {
    if (memaddr_changed)
        sprintf(mem_changed, "addr: 0x%04x, before: 0x%08x, after: 0x%08x", memaddr_changed, memval_before, memval_after);
    else
        strcpy(mem_changed, "addr: 0xffff, before: 0xffffffff, after: 0xffffffff");
}
