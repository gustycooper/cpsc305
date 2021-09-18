#define NAME_LEN 32

enum pp_cell_type { MT_CELL, POK_CELL, FIGHT_CELL };

struct pokmon {
    char name[NAME_LEN];
    int goodness;
    struct pokmon *next;
};

struct fighter {
    char name[NAME_LEN];
    int num_pokmon;
    struct pokmon *fighters;
};

// pokey play cell
struct pp_cell {
    enum pp_cell_type type;
    struct pokmon *pm;
    struct fighter  *ft;
};

struct player {
    char name[NAME_LEN];
    int energy;
    int num_pokmon;
    struct pokmon *pm;
};

int fight(struct pokmon *p1, struct pokmon *p2);
struct pokmon *new_pokmon(char *name, int goodness);
struct pp_cell *new_pp_cell(enum pp_cell_type t, void *p);
int read_pokmons(char *filename, struct pokmon *head);
struct pokmon *get_pokmon(struct pokmon *head);
struct fighter *new_fighter(char *name, struct pokmon *fighters);
