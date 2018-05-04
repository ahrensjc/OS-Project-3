#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main( int argc, char* argv[])
{
  int bigArray[1000000];
  int hugeArray[1000][1000];
  
  for(int i = 0; i < 1000000; i++)
  {bigArray[i] = i;}
  
  for(int i = 0; i < 1000; i++)
  {
    for(int j = 0; j < 1000; j++)
    {
      hugeArray[i][j] = i * j;
    }
  }
  
  sleep(3);
  
  printf("Big function is done\n");
  
  return 0;
}