enum sort_type { AGE, GPA, NAME };



typedef struct {
  char name[12];
  int age;
  double gpa;
} student;

student *new_student(char *n,int a,double g);
student **read_students(char *filename, int *num);
student *highest_gpa(student **students, int num);
void print_student(student *s);
void sort(student **students, int num, enum sort_type st);
