enum material {PAPER, GLASS, PLASTIC};
typedef enum {TEA, COFFEE, WATER, MILK, MIXED} contents;

typedef struct {
   int max_oz;
   int cur_oz;
   contents contents;
   enum material material;
   char color[16];    // "red, "pink", etc.
} cup;

void print_cup(cup *c);
cup *new_cup(int max, int cur, contents contents, enum material m, char *color);
int fill(cup *c, contents v, int oz);
cup *find_fullest(cup **ca, int num_eles);
void sort_cups(cup **ca, int num_eles);
void print_cups(cup **ca, int num_eles);
