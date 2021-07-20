// define part type
typedef struct {
  char name[80]; // name of part
  int number;    // bar code number of part
  double cost;   // cost of part
  int count;     // count of part in inventory
} part;
// define function prototypes
void print_part(part p);
void print_global_part();
