/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{  
  printf("Scan test:\n");
  int i;
  float f;
 
  char *line = "12 3.2\n";
  printf("Test line: \"%s\"\n", line);

  sscanf(line, "%d %f", &i, &f);
  printf("Result: %d %f\n", i, f);

  return 0;
}
