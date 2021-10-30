#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "cpu.h"
#include "dict.h"

// TODO 1. Update memory[MAX_MEM] to use a #define
// TODO 2. Update memory access functions to ensure address < MAX_MEM

unsigned char memory[32*1024]; // 32K bytes

void memory_store(int address, unsigned char value) {
    memory[address] = value;
}

void memory_store_word(int address, unsigned int value) {
    memory[address    ] = value >> 24 & 0xff;
    memory[address + 1] = value >> 16 & 0xff;
    memory[address + 2] = value >> 8  & 0xff;
    memory[address + 3] = value       & 0xff;
}

unsigned char memory_fetch(int address) {
    return memory[address];
}

unsigned int memory_fetch_word(int address) {
    return memory[address]     << 24 |
           memory[address + 1] << 16 |
           memory[address + 2] << 8  |
           memory[address + 3];
}

// read/write 32-bit value from/to memory
void system_bus(int address, int *value, int control) {
    if (control == READ)
        *value = memory_fetch_word(address);
    else
        memory_store_word(address, *value);
}

// read/write byte (8-bit) value from/to memory
void system_bus_b(int address, unsigned char *value, int control) {
    if (control == READ)
        *value = memory_fetch(address);
    else
        memory_store(address, *value);
}

void addresult(char *result);
static char result[80];

void dump_memory(int start_address, int num_bytes) {
    int start_boundary = start_address - (start_address % 8);
    int boundary_bytes = num_bytes + (start_address - start_boundary);
    for (int i = start_boundary; i < start_boundary+boundary_bytes; i+=8) {
        sprintf(result, "0x%04x (0d%04d) 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x ", 
               i, i,
               memory[i  ], memory[i+1], memory[i+2], memory[i+3],
               memory[i+4], memory[i+5], memory[i+6], memory[i+7]);
        addresult(result);
    }
}

void dump_memory_word(int start_address, int num_words) {
    for (int i = start_address; i < start_address+num_words; i+=16) {
        sprintf(result, "0x%04x (0d%04d) 0x%08x 0x%08x 0x%08x 0x%08x  ", 
               i, i,
               memory[i   ] << 24 | memory[i+1 ] << 16 | memory[i+2 ] << 8 | memory[i+3 ],
               memory[i+4 ] << 24 | memory[i+5 ] << 16 | memory[i+6 ] << 8 | memory[i+7 ],
               memory[i+8 ] << 24 | memory[i+9 ] << 16 | memory[i+10] << 8 | memory[i+11],
               memory[i+12] << 24 | memory[i+13] << 16 | memory[i+14] << 8 | memory[i+15]);
        addresult(result);
    }
}

#define MAX_LINE 100
static char buf[MAX_LINE];
/*
 File Format - Each line is one of the following
.data - continue address from previous
.data number - number is address
.text - continue address from previous
.text number - number is address
.stak number - number is address put into r13 (sp)
number - number is 32-bit value to be placed in memory
The first .text encountered set the pc to the address
 */
void load_memory(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        sprintf(result, "File %s not found!", filename);
        addresult(result);
        return;
    }
    int address = 0;
    int symbols = 0;
    while (fgets(buf, MAX_LINE, fp)) {
        int pos = 0, base = 10, mem_value = 0, directive = 0;
        buf[strcspn(buf, "\n")] = '\0';
        //printf("%s\n", buf);
        if (buf[0] == '.') { // directive 
            directive = 1;
            if (strlen(buf) > 5) { // directive has a number
                if (buf[6] == '0' && buf[7] =='x') {
                    base = 16;
                    pos = 8;
                }
                else 
                    pos = 6;
            }
            else
                directive = 2; // directive without a number
        }
        else if (symbols) { // processing symbols
            char symbol[40];
            int value;
            sscanf(buf, "%s %d", symbol, &value);
            dictput(symbol, value);
            // printf("%s %d\n", symbol, value);
        }
        else { // number
            if (buf[0] == '0' && buf[1] =='x') {
                base = 16;
                pos = 2;
            }
        }
        if (directive) {
            if (directive == 1) { // directive with number
                int taddress = (int)strtol(buf+pos, NULL, base);
                if (buf[1] == 's') // .stack directive
                    set_reg(13, taddress);
                else // either .text or .data directive
                    address = taddress;
            }
            if (buf[1] == 't') // .text
                set_reg(15, address);
            else if (buf[1] == 'y') // .ymbl for symbols
                symbols = 1;
        }
        else { // number
            mem_value = (int)strtoul(buf+pos, NULL, base);
            memory_store_word(address, mem_value);
            address += 4;
        }
    }
    fclose(fp);
    sprintf(result, "File %s loaded!", filename);
    addresult(result);
}
