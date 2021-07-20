#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * split str using tok. Simliar to Java's str.split(" ") except the
 * split string is a C tok and not a regular expression.  split can
 * cause memory leaks because it mallocs memory that is not returned.
 * Java has a garbage collector to fix these leaks.
 * 
 * @param char str[] the string that is to be split
 * @param char tok[] the tokens to use when splitting. 
 * @return an array of strings
 * 
 * @example
 * char **strings = split("This is a sample string."," ");
 * strings[0] is "This"
 * strings[1] is "is"
 * strings[2] is "a"
 * strings[3] is "sample"
 * strings[4] is "string."
 */
char **split(char str[], char tok[], int *num_elements) {
    // Copy str into array to handle a string literal passed
    char count_str[255];    // 255 character input string - ugly
    strcpy(count_str, str);
    char *tokens[100];      // 100 tokens in string - ugly 
    char * pch;
    int strings_in_str = 0; // count resulting strings for malloc
    pch = strtok(count_str,tok);
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

int main() {
  int num_strs;
  char **split_array = split("This is a sample string", " ", &num_strs);
  for (int i = 0; i < num_strs; i++)
    printf("%s\n", split_array[i]);
  return 0;
}
