#include <stdlib.h>
#include <string.h>
#include "split.h"

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
