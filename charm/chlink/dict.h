struct elem {
    struct elem *next;
    char *key;
    int value;
    sections section;
    char *fn; // filename with symbol
};

struct dict {
    struct elem **dict; // dictionary of elements
    int num_elems;     // num elems in dictionary
};

int dictput(struct dict *d, const char *, int, sections, char *fn);
int dictget(struct dict *d, const char *);
struct elem *dictgetelem(struct dict *d, const char *);
void dictshow(struct dict *d);
void dictsymbols(struct dict *d);
void dictrelocate(struct objinfo *o);
struct dict *dictinit();
