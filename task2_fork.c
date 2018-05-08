/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation on linux VM's, generates child processes of multiple sizes and tests
  the space it takes to allocate them.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <string.h>

int GetRamInKB();
void print_memory(pid_t pid_target);

int main(int argc, char* argv[])
{
  pid_t pid;
  
  //fork a child process
  //GetRamInKB();
  print_memory(getpid());
  
  
  // The following is a nested set of forks
  // This forks three children, but only from the parent
  // The children never fork
  pid = fork();

  if(pid < 0) //fork failed
  {
    fprintf(stderr, "Fork Failed\n");
    return 1;
  }
  else if(pid == 0) //child process 1
  {  
    printf("I am the first child %d mypid is %d\n", pid, getpid());
    print_memory(getpid());
    exit(0);
  }
  else //parent process
  {
    wait(NULL);
    printf("I am the parent %d, mypid is %d\n", pid, getpid());
    print_memory(getpid());

    pid = fork();
    if(pid < 0) //fork failed
      {
        fprintf(stderr, "Fork Failed\n");
        return 1;
      }
    else if(pid == 0) //child process 2
      {  
        printf("I am the second child %d mypid is %d\n", pid, getpid());
        print_memory(getpid());
        exit(0);
      }
    else //parent process
      {
        wait(NULL);
        printf("I am the parent %d, mypid is %d\n", pid, getpid());
        print_memory(getpid());
        pid = fork();
        if(pid < 0) //fork failed
          {
            fprintf(stderr, "Fork Failed\n");
            return 1;
          }
        else if(pid == 0) //child process 3
          {  
            printf("I am the third child %d mypid is %d\n", pid, getpid());
            print_memory(getpid());
            exit(0);
          }
        else //parent process
          {
            wait(NULL);
            printf("I am the parent %d, mypid is %d\n", pid, getpid());
            print_memory(getpid());
            
          }
      }
  }
  
  
  return 0;
}


// Print the memory usage of a target process
void print_memory(int pid_target){

  FILE *fp;
  char path[50];
  char pidString[10];
  char username[20];

  // Generate command to get the current user's processes
  // Get them in the format "PID RSS VSZ %MEM"
  getlogin_r(username, 20);
  strcpy(path, "/usr/bin/ps -o pid,rss,vsz,%mem -u ");
  strcat(path, username);

  char * lineBuf = NULL; // Stores the whole line
  char * lineData = NULL; // For trimming whitespace
  size_t len = 0;
  ssize_t read;
  int pid, rss, vsz;
  float pmem = 1.0;


  /* Open the command for reading. */
  fp = popen(path, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  read = getline(&lineBuf, &len, fp); // Trim first line (it only contains column headers)
  while ((read = getline(&lineBuf, &len, fp)) != -1) { // Scan the rest of the output line by line
    sscanf(lineBuf, "%d %d %d %f", &pid, &rss, &vsz, &pmem); // Read each line according to the format spec
    if(pid_target==pid){ // If we found the process we're looking for, print its info out
      printf("PID: %d\tRSS: %d\tVSZ: %d\t%%mem: %f\n", pid, rss, vsz, pmem);
    }
  }

}

