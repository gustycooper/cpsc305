typedef struct {
    int opcode;
    int rd;
    int rm;
    int rn;
    int flag;
    int address;
    int immediate16;
    int immediate20;
    int offset;
    int condition;
} decoded;

decoded *decode(unsigned int inst);
char *disassemble(unsigned int inst);
