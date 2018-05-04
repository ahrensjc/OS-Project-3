/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation for linuxVM's, shouldn't allocate a ton of space but is more processor   heavy.
*/

#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double random_double();

int main(int argc, char *argv[])
{
  int inCircle = 0;
  //int checker = 0;
  double pi;
 
  /* seed the random number generator */
	srandom((unsigned)time(NULL));


	#pragma omp parallel for
   for(int i = 0; i < 50000000; i++)
    {
      double x = random_double();
      double y = random_double();
      double distance = sqrt(x*x + y*y);
      //printf("x: %f,  y: %f,   distance: %f\n", x, y, distance);
      if(distance <= 1.0)
      {
        inCircle++;
      }
      //checker++;
    }

  //printf("num of things in circle: %d,   checker: %d\n", inCircle, checker);
	pi = (4.0 * inCircle / 50000000);

  printf("Pi approximation: %f\n", pi);
  
  

	return 0;
}

double random_double() //Generates a double precision random number
{
  return random() / ((double)RAND_MAX + 1);
}