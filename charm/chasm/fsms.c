#include "fsms.h"
#include "chasm_types.h"
#include <stdio.h>

/*
 * FSMs to recognize numbers, identifiers, registers, and instructions. 
 * Number formats: 123 -123 #123 #-123 0x123, #0x123
 * Identifier formats: abc ab12 ab_12
 * For numbers there are 8 numstates and 21 numletters. The variable numtrans[][] provides the state transition table
 * For idents there are 4 idstates and 4 idletters. The variable idtrans[][] provides the state transition table
 */

enum numstates  { start, num, dec, hex, neg, pnd, gud, gux, bad };

               //   0       1         2       3     4      5       6        7
enum numletters { zero, onetonine, hexatof, letx, pound, minus, backzero, other };

enum numstates numtrans[10][8] = 
               // 0  1-9 a-f  x   #   -  \0  oth
 /* start */  { {num,dec,bad,bad,pnd,neg,bad,bad},
 /* num   */    {dec,dec,bad,hex,bad,bad,gud,bad},
 /* dec   */    {dec,dec,bad,bad,bad,bad,gud,bad},
 /* hex   */    {hex,hex,hex,bad,bad,bad,gux,bad},
 /* neg   */    {dec,dec,bad,bad,bad,bad,bad,bad},
 /* pnd   */    {num,dec,bad,bad,bad,neg,bad,bad},
 /* neg   */    {dec,dec,bad,bad,bad,bad,bad,bad},
              };
// Maps a char to one of the numletters
enum numletters chartolet(char c) {
    if (c == '0')
        return zero;
    else if (c >= '1' && c <= '9')
        return onetonine;
    else if (c >= 'a' && c <= 'f')
        return hexatof;
    else if (c == 'x')
        return letx;
    else if (c == '#')
        return pound;
    else if (c == '-')
        return minus;
    else if (c == '\0')
        return backzero;
    else
        return other;
}

// Input: string that may be a number
// Ouput: 0 - not a number, 10 - decimal number, 16 - hex number
int isnumber(char *n) {
    enum numstates s = numtrans[start][chartolet(*n++)];
    while (s != bad && s != gud && s != gux)
        s = numtrans[s][chartolet(*n++)];
    return s == gud ? 10 : s == gux ? 16 : 0;
}
    
enum idstates { idstart, id, idgud, idbad };
enum idletters { idlets, idnums, idbackzero, idother };
enum idstates idtrans[10][4] =
             //    a-z   0-1    \0   oth
 /* idstart */ { {id   ,idbad,idbad,idbad},
 /* id      */   {id   ,id   ,idgud,idbad},
               };
// Maps a char to one of the idletters
int idchartolet(char c) {
    if (c >= 'a' && c <= 'z')
        return idlets;
    else if ((c >= '0' && c <= '9') || c == '_')
        return idnums;
    else if (c == '\0')
        return idbackzero;
    else
        return idother;
}
// Input: string that may be an identifier
// Ouput: 0 - not an id, 1 - identifier
int isid(char *n) {
    enum idstates s = idtrans[idstart][idchartolet(*n++)];
    while (s != idbad && s != idgud)
        s = idtrans[s][idchartolet(*n++)];
    return s == idgud ? 1 : 0;
}

enum regstates { regstart, regg, reg0, reg1, reg2, reggud, regbad };
enum regletters { regr, reglet0, reglet1, reg2to5, reg6to9, regbackzero, regother };
enum regstates regtrans[10][7] =
             //     r        0       1       2-5     6-9    \0     oth
 /* regstart */ { {regg   , regbad, regbad, regbad, regbad,regbad,regbad},
 /* regg      */  {regbad,   reg0,   reg1,   reg0,   reg0,regbad,regbad},
 /* reg0     */   {regbad, regbad, regbad, regbad, regbad,reggud,regbad},
 /* reg1     */   {regbad,   reg2,   reg2,   reg2, regbad,reggud,regbad},
 /* reg2     */   {regbad, regbad, regbad, regbad, regbad,reggud,regbad},
               };
// Maps a char to one of the idletters
int regchartolet(char c) {
    if (c == 'r')
        return regr;
    else if (c == '0')
        return reglet0;
    else if (c == '1')
        return reglet1;
    else if (c >= '2' && c <= '5')
        return reg2to5;
    else if (c >= '6' && c <= '9')
        return reg6to9;
    else if (c == '\0')
        return regbackzero;
    else
        return regother;
}

// Input: string that may be a register
// Ouput: 0 - not a reg, 1 - reg
int isreg(char *n) {
    enum regstates s = regtrans[regstart][regchartolet(*n++)];
    while (s != regbad && s != reggud)
        s = regtrans[s][regchartolet(*n++)];
    return s == reggud ? 1 : 0;
}

/* 
FSM Tables for each category of instruction
arilog
          data text labl stri inst comm regg coma numb lbrk rbrk excl iden badd endd
s0                                       s1
s1                                            s2
s2                                       s3
s3                                            s4
s4                                       s5        s5
s5                                                                               ok

movcmp
          data text labl stri inst comm regg coma numb lbrk rbrk excl iden badd endd
s0                                       s1
s1                                            s2
s2                                       s3        s4                  s5
s3                                                                               ok - mov r,r
s4                                                                               ok - mov r,#n
s5                                                                               ok - mov r,id

branch
          data text labl stri inst comm regg coma numb lbrk rbrk excl iden badd endd
s0                                                      s1        s5   s4
s1  [                                    s2
s2  [r                                                       s3      
s3  [r]                                                                          ok - bal [r]
s4  ident                                                                        ok - bal ident
s5  !                                                                  s6
s6  !ident                                                                       ok - bal !ident

ldrstr
          data text labl stri inst comm regg coma numb lbrk rbrk excl iden badd endd
s0  start                                s1
s1  r                                         s2
s2  r,                                             s8   s3             s8
s3  r,[                                  s4
s4  r,[r                                      s5             s9      
s5  r,[r,                                s6        s7                          
s6  r,[r,r                                                   s11
s7  r,[r,#n                                                  s10
s8  r,addr                                                                       ok - ldr r,addr
s9  r,[r]                                                                        ok - ldr r,[r]
s10 r,[r,#n]                                                      s12            ok - ldr r,[r,#n]
s11 r,[r,r]                                                       s13            ok - ldr r,[r,r]
s12 r,[r,#n]!                                                                    ok - ldr r,[r,#n]!
s13 r,[r,r]!                                                                     ok - ldr r,[r,r]!

miscos
          data text labl stri inst comm regg coma numb lbrk rbrk excl iden badd endd
s0                                                 s1                    
s1  numb                                                                         ok - ker numb

*/

int ariloga[10][15] =
      //  dat txt lab str ins cmt reg cma num   [   ]   ! idt bad end
/* 0*/  { {-1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 1*/    {-1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1},
/* 2*/    {-1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1},
/* 3*/    {-1, -1, -1, -1, -1, -1, -1,  4, -1, -1, -1, -1, -1, -1, -1},
/* 4*/    {-1, -1, -1, -1, -1, -1,  5, -1,  5, -1, -1, -1, -1, -1, -1},
/* 5*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x00},
  };

int movcmpa[10][15] = 
      //  dat txt lab str ins cmt reg cma num   [   ]   ! idt bad end
/* 0*/  { {-1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 1*/    {-1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1},
/* 2*/    {-1, -1, -1, -1, -1, -1,  3, -1,  4, -1, -1, -1,  4, -1, -1},
/* 3*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x60},
/* 4*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x70},
        };

int brancha[10][15] = 
      //  dat txt lab str ins cmt reg cma num   [   ]   ! idt bad end
/* 0*/  { {-1, -1, -1, -1, -1, -1, -1, -1,  4,  1, -1,  5,  4, -1, -1},
/* 1*/    {-1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1},
/* 2*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1},
/* 3*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x90}, // b [r]
/* 4*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x80}, // b label
/* 5*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  6, -1, -1},
/* 6*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0xa0}, // b !label
        };

int ldrstra[17][15] = 
//        dat txt lab str ins cmt reg cma num   [   ]   ! idt bad end
/* 0*/  { {-1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1},
/* 1*/    {-1, -1, -1, -1, -1, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1},
/* 2*/    {-1, -1, -1, -1, -1, -1, -1, -1,  8,  3, -1, -1,  8, -1, -1},
/* 3*/    {-1, -1, -1, -1, -1, -1,  4, -1, -1, -1, -1, -1, -1, -1, -1},
/* 4*/    {-1, -1, -1, -1, -1, -1, -1,  5, -1, -1,  9, -1, -1, -1, -1},
/* 5*/    {-1, -1, -1, -1, -1, -1,  6, -1,  7, -1, -1, -1, -1, -1, -1},
/* 6*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1, -1, -1},
/* 7*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, -1},
/* 8*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x00}, // r,addr
/* 9*/    {-1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1, -1, -1, -1, 0x01}, // r,[r]
/*10*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, -1, -1, 0x02}, // r,[r,#n]
/*11*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 13, -1, -1, 0x03}, // r,[r,r]
/*12*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x04}, // r,[r,#n]!
/*13*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x05}, // r,[r,r]!
/*14*/    {-1, -1, -1, -1, -1, -1, 16, -1, 15, -1, -1, -1, -1, -1, -1},
/*15*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x06}, // r,[r],#n
/*16*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x07}, // r,[r],r
        };

int miscosa[10][15] = 
      //  dat txt lab str ins cmt reg cma num   [   ]   ! idt bad end
/* 0*/  { {-1, -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, -1, -1, -1, -1},
/* 1*/    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0x00},
        };

int isinst(struct toki_t t) {
    int (*trans)[15] = ariloga;
    if (t.instcate == arilog)
        trans = ariloga;
    else if (t.instcate == movcmp)
        trans = movcmpa;
    else if (t.instcate == branch)
        trans = brancha;
    else if (t.instcate == ldrstr)
        trans = ldrstra;
    else if (t.instcate == miscos)
        trans = miscosa;
    int i = 1, opcode = t.instopcd;
    int s = trans[0][t.toks[i++].toktype];
    while (s >= 0 && t.toks[i].toktype != endd && t.toks[i].toktype != comment)
        s = trans[s][t.toks[i++].toktype];
    return s < 0 ? -1 : opcode | trans[s][endd];
}
