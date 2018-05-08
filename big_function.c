/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's, generates a large array using malloc.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#define mem_array_size 50000

int main( int argc, char* argv[])
{
  int i;
  int *addresses[mem_array_size];
  int *temp;
  for(i = 0; i < mem_array_size; i++){
    temp = (int *)malloc(1000*sizeof(int));
    addresses[i] = temp;
  }
  printf("Child done allocating\n");
  sleep(10);
  for(i=0; i < mem_array_size; i++){
    free(addresses[i]);
  }
  exit(0);
  
  return 0;
}

