#define MAX_LINE 100

typedef enum sections { DATA=1, TEXT, STACK } sections;
    
typedef struct vals {
    int address;
    int reladdr;
    int val;
    int relval;
    sections section;
    char *extern_symbol;
} vals;

typedef struct objinfo {
    int oldstartDaddr; // initial start address of data
    int startDaddr;    // relocated start addr of data
    int numdata;       // number of 32-bit data elements
    int oldstartIaddr; // initial start address of text
    int startIaddr;    // relocated start addr of text
    int numtext;       // number of 32-bit text elements
    int numtotal;      // total number of 32-bit elements (data and text)
    int stack;         // address of stack
    struct dict *d;    // dictionary for object symbols
    char *fn;          // object file name
    vals vals[1000];   // data and text values
} objinfo;
