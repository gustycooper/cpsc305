// define part type
typedef struct {
  char name[80]; // name of part
  int number;    // bar code number of part
  double cost;   // cost of part
  int count;     // count of part in inventory
} part;
// define function prototype for update_cost
void update_cost1(part p, double percent);
void update_cost2(part *p, double percent);
void print_part(part p);
