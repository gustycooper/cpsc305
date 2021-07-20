#ifndef CHASM_TYPES_H
#define CHASM_TYPES_H
#define MAX_TOKENS 15

enum inst_c {
    ldrstr, arilog, movcmp, branch, 
    miscos, 
};

enum inst_t {
    ldr, ldb, str, stb, 
    add, sub, mul, dIv, 
    and, orr, eor, adc, 
    sbc, adf, sbf, muf, 
    dif, adi, sbi, mov, 
    mva, cmp, tst, teq, 
    shf, sha, rot, bal, 
    beq, bne, blt, ble, 
    bgt, bge, blr, ker,
    srg, ioi,
};

struct inst_info {
    int inst_int;
    char inst_str[4];
    enum inst_t inst_t;
    enum inst_c inst_c;
    int opcode;
};

enum toks_t { 
    data, text, label, string, inst, 
    comment, reg, comma, number, 
    leftbrack, rightbrack, exclam, ident, 
    err, endd
};

struct tokv_t {
    char *tok_str;
    enum toks_t toktype;
    int tokv;
};

struct toki_t {
    int  numtoks;                   // num of toks on line
    int  linenum;                   // line number
    int  address;                   // mem address of this line
    enum toks_t linetype;           // type of line
    enum inst_t insttype;           // if line is inst_t, is ins type - ldr, mov, etc
    enum inst_c instcate;           // if line is inst_t, is ins categor - ldrstr, arilog, etc
    int  instopcd;                  // if line is inst_t, is ins opcode
    struct tokv_t toks[MAX_TOKENS]; // values for each token on line
};

#endif
