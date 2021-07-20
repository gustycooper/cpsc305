// define part type
typedef struct {
  char name[80]; // name of part
  int number;    // bar code number of part
  double cost;   // cost of part
  int count;  // count of part in inventory
} part;
// define function prototypes
part *new_part(char *name, int number, double cost, int count);
void update_cost(part *p, double percent);
void print_part(part p);
int read_parts(char *filename, part parts[], int *num_of_parts);
int read_parts_fancy(char *filename, part parts[], int *num_of_parts);
int write_parts(char *filename, part parts[], int num_of_parts);
part *find_longest(part *parts, int num_of_parts);
void print_expensive(part *parts, int num_of_parts);
