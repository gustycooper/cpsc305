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
