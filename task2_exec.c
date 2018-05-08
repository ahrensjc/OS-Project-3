/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation on linux VM's, generates child process and calls execv on it to see how much a 
  child that will call another function will allocate.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <string.h>

void print_memory(pid_t pid_target);

int main(int argc, char* argv[])
{
  pid_t pid;
  
  printf("Ram used by main:\n");
  print_memory(getpid());

  // Fork a child  
  pid = fork();

  if(pid < 0) //fork failed
  {
    fprintf(stderr, "Fork Failed\n");
    return 1;
  }
  else if(pid == 0) //child process
  {  
    printf("I am the child %d mypid is %d\n", pid, getpid());
    print_memory(getpid());
    
    char* arr[] = {NULL};
    //execv("big_function", arr);
    execv("empty_function", arr);
  }
  else //parent process
  {
    sleep(3);
  
    printf("I am the parent %d, mypid is %d\n", pid, getpid());
    print_memory(getpid());
    printf("Child memory:\n");
    print_memory(pid);

  }
  
  return 0;
}

void print_memory(int pid_target){

  FILE *fp;
  char path[50];
  char pidString[10];
  char username[20];

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
  read = getline(&lineBuf, &len, fp); // Trim first line
  while ((read = getline(&lineBuf, &len, fp)) != -1) {
    sscanf(lineBuf, "%d %d %d %f", &pid, &rss, &vsz, &pmem);
    if(pid_target==pid){
      printf("PID: %d\tRSS: %d\tVSZ: %d\t%%mem: %f\n", pid, rss, vsz, pmem);
    }
  }

}
