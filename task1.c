/*
  Written By: James Ahrens & Isaac Hendrickson
  May 4, 2018
  Desc: Used to test memory allocation on linux VM's, generates child processes of multiple sizes and tests
  the space it takes to allocate them.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <signal.h>

// How many integer arrays to allocate
#define mem_array_size 50000

void print_memory(int pid_target);

int main( int argc, char* argv[])
{
  pid_t pid1, pid2, pid3;
  int i;

  printf("Memory initially allocated to main process:\n");
  print_memory(getpid());

  // Fork first child
  pid1 = fork();
  if (pid1 < 0){
    printf("Fork failed!\n");
    exit(1);
  }

  else if (pid1==0){ // Child process
    while(1){} // Spin until killed
  }
  else{ // Parent process
    // Check memory on first child process
    printf("Memory allocated to first child (no background processes)\n");
    print_memory(pid1);
    kill(pid1, SIGKILL); // Kill child process
  }

  // Fork second child
  pid2 = fork();
  if (pid2 < 0){
    printf("Fork failed!\n");
    exit(1);
  }

  else if (pid2==0){ // Child process
    // Use a lot of memory
    // This loops through and runs malloc A LOT
    int *addresses[mem_array_size];
    int *temp;
    for(i = 0; i < mem_array_size; i++){
      temp = (int *)malloc(500000*sizeof(int));
      addresses[i] = temp;
    }
    printf("Child done allocating\n");
    sleep(10);
    for(i=0; i < mem_array_size; i++){
      free(addresses[i]); // Free allocated memory
    }
    exit(0);
  }
  else{ // Parent process
    sleep(3);
    // Check memory on second child process
    printf("Memory allocated to second child process\n");
    print_memory(pid2);
    kill(pid2, SIGKILL);
  }


  printf("Memory allocated to main after using up a lot of storage\n");
  print_memory(getpid());

  // Fork third child
  pid3 = fork();
  if (pid3 < 0){
    printf("Fork failed!\n");
    exit(1);
  }

  else if (pid3==0){ // Child process
    while(1){} // Spin until killed
  }
  else{ // Parent process
    // Check memory on third child process
    printf("Memory allocated to third child (GIANT background process)\n");
    print_memory(pid3);
    kill(pid3, SIGKILL); // Kill third child
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
