enum dicttype { INT, STRING, LOCAL };
struct dictval {
    char *key;
    enum dicttype type;
    int ivalue;
    char *svalue;
    sections section;
};
int dictput(const char *, int, sections);
int dictputval(struct dictval *);
int dictget(const char *);
int dictgetval(const char *, struct dictval *);
void dictprint(int);
