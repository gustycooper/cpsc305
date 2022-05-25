#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/stat.h>
#include <libgen.h>
#include <fcntl.h>
#include "chlink.h"
#include "dict.h"

static char buf[MAX_LINE];

static objinfo **o;  // array of pointers to objinfo

struct dict *bigd;   // global dictionary to detect duplicates accross objects

int verbose = 0;  // implement a -v option

int readfile(char *fn, vals *vals, struct dict *d) {

    int i = 0;
    int address = 0;
    int symbols = 0;
    sections section = DATA;
    FILE *fp = fopen(fn, "r");
    if (fp == NULL) {
        fprintf(stderr, "File %s not found!\n", fn);
        exit(1);
    }
    int firsttext = 1, firstdata = 1;
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
// Symbol lines have the following format: identifier address section
// gusty 0x100 1
// else 0x220 2
// Section 1 is data and section 2 is text
            char symbol[40];
            int value, symsection;
            sscanf(buf, "%s %x %d", symbol, &value, &symsection);
            if (dictput(d, symbol, value, symsection, fn))
                fprintf(stderr, "duplicate symbol: %s, file: %s\n", symbol, fn);
            if (dictput(bigd, symbol, value, symsection, fn)) {
                struct elem *e = dictgetelem(bigd, symbol);
                fprintf(stderr, "duplicate symbol: %s, file: %s; ", symbol, fn);
                fprintf(stderr, "1st defined in file: %s\n", e->fn);
                exit(0);
            }
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
                if (buf[1] == 't' && firsttext) {// .text
                    address = taddress;
                    firsttext = 0;
                }
                else if (buf[1] == 'd' && firstdata) {
                    address = taddress;
                    firstdata = 0;
                }
            }
            if (buf[1] == 't') { // .text
                section = TEXT;
            }
            else if (buf[1] == 'd') { // .data
                section = DATA;
            }
            else if (buf[1] == 'y') // .ymbl for symbols
                symbols = 1;
        }
        else if (symbols) {
            continue;
        }
        else { // number
// Instructions may reference external symbols. When so, the line has the following format
// 0x101XXXXX gusty
// The previous line is ldr r0, gusty where gusty is external
// chlinker replacess XXXXX with the address of gusty, which is in another .o file
            if (buf[5] == 'X') { // external symbol
                vals[i].extern_symbol = strdup(buf+11);
                buf[5] = 0; // replace the first X in the string with null
                mem_value = (int)strtoul(buf+pos, NULL, base);
                vals[i].val = mem_value; // this val is 0x101 - the ldr r0 portion of the inst
            }
            else {
                mem_value = (int)strtoul(buf+pos, NULL, base);
                vals[i].val = mem_value;
            }
            vals[i].address = address;
            vals[i].section = section;
            address += 4;
            i++;
        }
    }
    fclose(fp);
    return i;
}

/*
 sectaddr parameters:
 v - array of values in the object. Each value is 32-bits.
 len - number of values in object.
 section - type section - DATA or TEXT. Object has both.
 Sets fa to address of section in object.
 Returns number of bytes in section.
 */
int sectaddr(vals *v, int len, sections section, int *fa) {
    int lastaddress = 0, firstaddress = -1;
    for (int i = 0; i < len; i++) {
        if (v[i].section == section) {
            if (firstaddress == -1)
                firstaddress = v[i].address;
            lastaddress = v[i].address;
        }
    }
    *fa = firstaddress;
    return lastaddress - firstaddress;
}

/*
 relocate parameters:
 v - array of vals to be relocated
 oda - old data address
 nda - new data address
 ota - old text address
 nta - new text address
 */
void relocate(vals *v, int s, int oda, int nda, int ota, int nta) {
    int rda = 0, rta = 0;
    for (int i = 0; i < s; i++) {
        if (v[i].extern_symbol) {
            int addr = dictget(bigd, v[i].extern_symbol);
            if (verbose)
                printf("extern: %s, 0x%x\n", v[i].extern_symbol, addr);
        }
        if (v[i].section == DATA) {
            v[i].reladdr = nda + rda;
            rda += 4;
        }
        if (v[i].section == TEXT) {
            v[i].reladdr = nta + rta;
            rta += 4;
        }
        int opcode = v[i].val >> 24 & 0xff; // mask with 0xff in case >> is arithmetic
        if (opcode == 0x10 || opcode == 0x20 || // ldr/str rd, address
            opcode == 0x30 || opcode == 0x40 || // ldb/stb rd, address
            opcode == 0x81) {                   // mva rd, address
            int addr = v[i].val & 0xfffff; // bit 0-19 have data addr
// printf and scanf are in ChOS at addresses 0x7000 and 0x7050.
// When programming, you enter the following to reference printf (for example).
// .text 0x7000
// .label printf
// .text 0x200 <-- your code in this section
// blr printf
// chasm translates the blr printf to be 0x9707000
// The following if statement prevents relocating this call to printf / scanf
// This is a bit awkward, but good for now.
            if (addr == 0x7000 || addr == 0x7050) { // printf and scanf are hardcoded addresses
                v[i].relval = v[i].val;
            }
            else {
                int offset = addr - oda;
                addr = nda + offset;
                v[i].relval = (v[i].val & 0xfff00000) | addr;
            }
        }
        else if (opcode >= 0x90 && opcode <= 0x97) { // branch with address
            int addr = v[i].val & 0xfffff; // bit 0-19 have dest addr
            int offset = addr - ota;
            addr = nta + offset;
            v[i].relval = (v[i].val & 0xfff00000) | addr;
        }
        else {
            v[i].relval = v[i].val;
        }
    }

}

void showvals(vals *v, int s) {
    for (int i = 0; i < s; i++)
        printf("i: %d, v.val: 0x%08x, v.adr: 0x%08x, v.section: %d, v.reladdr: 0x%08x, v.relval: 0x%08x\n", i, v[i].val, v[i].address, v[i].section, v[i].reladdr, v[i].relval);
}

void showobjinfo(objinfo *o) {
    printf("Object file %s processed!\n", o->fn);
    printf("Dictionary\n");
    dictshow(o->d);
    showvals(o->vals, o->numtotal);
}

/*
 genobject parameters:
 o - array of objinfo used to generate object code
 section - DATA or TEXT to generate .data or .text
 addr - start address of the section
 returns the number of 32-bit words emitted
 */
int genobject(objinfo *o, sections section, int addr) {
    if (section == DATA)
        printf(".data 0x%x\n", addr);
    else
        printf(".text 0x%x\n", addr);
    int c = 0;
    for (int i = 0; i < o->numtotal; i++) {
        if (o->vals[i].section == section) {
            printf("0x%08x\n", o->vals[i].relval);
            c++;
        }
    }
    return c;
}

char *output = "a_out.o"; // default outfile for chlinker

/*
  % chlinker code1.o code2.o : link code1.o and code2.o into a_out.o
  % chlinker -v code1.o code2.o : link code1.o and code2.o into a_out.o, verbose mode
  % chlinker -o a.o code1.o code2.o : link code1.o and code2.o into a.o
  The long versions of the options are invoked a la 
  $ chlinker -verbose code1.o code2.o
  long_options allows you to put -v anywhere on the line. For example
  % chlinker code1.o code2.o -v
  However, since chlinker has multiple file arguments, you must put options first.
  % chlinker -v code1.o code2.o
  This is because getopts returns optind as 2 for both of these invocations
 */
static struct option long_options[] = {
  {"verbose",no_argument,       0,  'v' },
  {"outfile",required_argument, 0,  'o' },
  {0,        0,                 0,   0  }
};

/*
 get_opts - called to process command line options
 input
  count - copy of argc
  args  - copy of argv
 output
  listing - -lfile sets listing to "file"
  return of 0 is error
  return of non 0 is index in argv of assembly file name
 */
int get_opts(int count, char *args[]) {
    int opt, good = 1, long_index;
    while (good && (opt = getopt_long(count, args, "o:v", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'o':
                output = strdup(optarg);
                break;
            case 'v':
                verbose = 1;
                break;
            default:
                fprintf(stderr, "Invalid invocation.\n");
                good = 0;
                break;    
        }
    }
    if(good && optind > count-2) {
        fprintf(stderr, "chlinker: num args: %d, must link at least 2\n", optind);
        good = 0;
    }
    else if (good)
        good = optind;
    return good;

}

/*
 File Format - Each line is one of the following
.data - continue address from previous
.data number - number is address
.text - continue address from previous
.text number - number is address
.stak number - number is address put into r13 (sp)
hex number - is 32-bit value to be placed in memory
0x105XTERN gusty - is an instruction referencing the external symbol gusty
.ymbl - denotes object symbols follow
gusty 0x120 1 - the symbol gusty at address 0x120 is in data
else 0x240 2 - the symbol else at address 0x240 is in text
The first .text encountered set the pc to the address
 */
int main(int argc, char **argv) {
    int optind = get_opts(argc, argv);
    if (!optind) {
        fprintf(stderr, "Bad chasm command invocation!\n");
        return -1;
    }

    int numobjs = argc - optind;
    o = calloc(numobjs, sizeof(objinfo *)); // objinfo **o
    for (int i = 0; i < numobjs; i++) {
        o[i] = calloc(1, sizeof(objinfo));
        o[i]->d = dictinit();
    }

    bigd = dictinit();

    for (int i = 0; i < numobjs; i++) {
        o[i]->fn = argv[i+optind];
        struct dict *d = o[i]->d;
        vals *v = o[i]->vals;
        //printf("i in read loop: %d, %s, %p\n", i, o[i]->fn, v);
        o[i]->numtotal = readfile(o[i]->fn, v, d);
        //o[i-1]->numtotal = readfile(fp, v, o[i-1]->vals); <--- did not work
        // passed address of o[0]->vals when i was 2.
    }

    // chlinker writes to stdout. dup2 output to stdout
    int fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int dup2stat = dup2(fd, 1);
    if (fd == -1 || dup2stat == -1) {
        fprintf(stderr, "File %s not created!\n", output);
        return -1;
    }
    close(fd);

    int firstDaddr, firstTaddr;
    int newDaddr, newTaddr;
    int numDbytes, numTbytes;
    numDbytes = sectaddr(o[0]->vals, o[0]->numtotal, DATA, &firstDaddr);
    newDaddr = firstDaddr;
    numTbytes = sectaddr(o[0]->vals, o[0]->numtotal, TEXT, &firstTaddr);
    newTaddr = firstTaddr;
    for (int i = 0; i < numobjs; i++) {
        o[i]->oldstartDaddr = firstDaddr;
        o[i]->oldstartIaddr = firstTaddr;
        o[i]->startDaddr = newDaddr;
        o[i]->startIaddr = newTaddr;
        relocate(o[i]->vals, o[i]->numtotal, firstDaddr, newDaddr, firstTaddr, newTaddr);
        if (i == numobjs-1)
            break;
        newDaddr = newDaddr + numDbytes + 4;
        newTaddr = newTaddr + numTbytes + 4;
        numDbytes = sectaddr(o[i+1]->vals, o[i+1]->numtotal, DATA, &firstDaddr);
        numTbytes = sectaddr(o[i+1]->vals, o[i+1]->numtotal, TEXT, &firstTaddr);
    }

    if (verbose) {
        printf("showobjinfo\n");
        for (int i = 0; i < numobjs; i++)
            showobjinfo(o[i]);
    }

    for (int i = 1; i < numobjs; i++) {
        dictrelocate(o[i]);
    }
    for (int i = 0; i < numobjs; i++) {
        for (int j = 0; j < o[i]->numtotal; j++) {
            if (o[i]->vals[j].extern_symbol) {
                int addr = dictget(bigd, o[i]->vals[j].extern_symbol);
                if (addr == -1000001) {
                    printf("Undefined external symbol: %s in file %s\n", o[i]->vals[j].extern_symbol, o[i]->fn);
                    fprintf(stderr, "Undefined external symbol: %s in file %s\n", o[i]->vals[j].extern_symbol, o[i]->fn);
                    addr = 0xfffff;
                }
                int v = o[i]->vals[j].val;
                v = (v << 20) | addr;
                o[i]->vals[j].val = v;
                o[i]->vals[j].relval = v;
                if (verbose)
                    printf("EXTERN: %s, 0x%x, 0x%x\n", o[i]->vals[j].extern_symbol, addr, o[i]->vals[j].val);
            }
        }
    }
    for (int i = 0; i < numobjs; i++)
        genobject(o[i], DATA, o[i]->startDaddr);
    for (int i = 0; i < numobjs; i++)
        genobject(o[i], TEXT, o[i]->startIaddr);
    printf(".ymbl\n");
    dictsymbols(o[0]->d);
    for (int i = 1; i < numobjs; i++) {
        dictsymbols(o[i]->d);
    }

}
