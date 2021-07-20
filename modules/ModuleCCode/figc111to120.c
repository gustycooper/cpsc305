#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
struct point {
    int x;
    int y; 
};
typedef struct point point;
void point_print(point p) {
  printf("(%d, %d)\n", p.x, p.y);
}

typedef struct {
  char name[12];
  int age;
  double gpa;
} student;
student gusty = {"Gusty", 22, 3.25};

student *new_student(char *n,int a,double g) {
    student *s = malloc(sizeof(student));
    strcpy(s->name, n);
    s->age = a;
    s->gpa = g;
    return s;
}

typedef char* string;

typedef struct {
    char name[80];
    int number;
    double cost;
} part;
part parts[] = { {"bolt", 123, 1.25}, {"screw", 124, .45},{ "cassette", 125, 10.45} };

char **split(char str[], char tok[], int *num_elements) {
  char count_str[255];    // 255 character input string - ugly
  strcpy(count_str, str); // cpy str into array to handle string literals
  char *tokens[100];      // 100 tokens in string - ugly 
  int strings_in_str = 0; // count resulting strings for malloc
  char *pch = strtok(count_str,tok);
  while (pch != NULL) {
    tokens[strings_in_str] = pch;
    pch = strtok (NULL,tok);
    strings_in_str++;
  }
  char **array = malloc (sizeof (char *) * strings_in_str);
  if (!array)
    return NULL;
  for (int i = 0; i < strings_in_str; i++) {
    array[i] = malloc(strlen(tokens[i])+1);
    if (!array[i])
      return NULL;    // should make calls to free
    strcpy(array[i], tokens[i]);
  }
  *num_elements = strings_in_str;  
  return array;
}

enum dog_type {hound, poodle, terrier, shepard, beagle};

typedef struct dog {
  char *name;
  enum dog_type dt;
  union {
    int blue_shoes;
    int curly;
    int fox;
    int german;
    int snoopy;
    int extra;
  };
  int age;
  int toes;
} dog;

int main() {
  struct point p1 = {0, 1};
  point_print(p1);
  printf("sizeof(student): %lu\n", sizeof(student));
  string s = "Hello";
  printf("s: %s\n", s);
  student *st = new_student("Gusty", 22, 3.1);
  printf("st - name: %s, age: %d, gpa: %lf\n", st->name, st->age, st->gpa);
  part longest = parts[0];
  for (int j = 0; j < 3; j++) {
    printf("Counter Loop: %s %d %lf\n", parts[j].name, parts[j].number, parts[j].cost);
    if (strlen(longest.name) < strlen(parts[j].name))
      longest = parts[j];
  }	
  printf("longest name: %s\n", longest.name);
  const part *end = &parts[3];	
  for (part *p = parts; p <= end; p++)
    printf("Pointer Loop: %s %d %lf\n", p->name, p->number, p->cost);

  FILE *fp;
  fp = fopen("figc111to120students.txt", "r");
  char name[255];
  int age = 0;
  double gpa = 0;
  int i = 0;
  student students[200];
  while (fscanf(fp, "%s %d %lf", name, &age, &gpa) != EOF) {
    strcpy(students[i].name, name);
    students[i].age = age;
    students[i].gpa = gpa;
    i++;
  }
  printf("%s, %d, %0.2lf\n", students[1].name, students[1].age, students[1].gpa);
  fclose(fp);

  students[1].age = 0;

  fp = fopen("figc111to120students.txt", "r");
  i = 0;
  while (fscanf(fp, "%s %d %lf", students[i].name, &students[i].age, &students[i].gpa) != EOF)
    i++;
  printf("%s, %d, %0.2lf\n", students[1].name, students[1].age, students[1].gpa);
  fclose(fp);

  students[1].age = 0;

  fp = fopen("figc111to120students.txt", "r");
  i = 0; 
  int num_strs = 0;
  char line[100] = { 0 };
  while (fgets(line, 100, fp) != NULL) {
    char **split_array = split(line, " ", &num_strs);
    strcpy(students[i].name, split_array[0]);
    students[i].age = atoi(split_array[1]);
    students[i].gpa = atof(split_array[2]);
    i++;
  }
  printf("%s, %d, %0.2lf\n", students[1].name, students[1].age, students[1].gpa);
  fclose(fp);

  enum dog_type dt = poodle;
  dt = 100;
  for (int i = hound; i <= beagle; i++)
    printf("%d\n", i);

  dog fido = {"Fido", poodle, {5}, 4};
  switch (fido.dt) {
    case poodle:
      fido.curly = 3;
    case shepard:
      fido.german = 100;
    default:
      fido.extra = 1000;
  }
  printf("sizeof(fido): %lu, fido.curly: %d\n", sizeof(fido), fido.curly);
  return 0;
}
