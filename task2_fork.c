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
  
  pid = fork();

  if(pid < 0) //fork failed
  {
    fprintf(stderr, "Fork Failed\n");
    return 1;
  }
  else if(pid == 0) //child process
  {  
    printf("I am the first child %d mypid is %d\n", pid, getpid());
    print_memory(getpid());
    exit(0);
  }
  else //parent process
  {
    waitpid(pid, NULL, WNOHANG);
    printf("I am the parent %d, mypid is %d\n", pid, getpid());
    print_memory(getpid());

    pid = fork();
    if(pid < 0) //fork failed
      {
        fprintf(stderr, "Fork Failed\n");
        return 1;
      }
    else if(pid == 0) //child process
      {  
        printf("I am the second child %d mypid is %d\n", pid, getpid());
        print_memory(getpid());
        exit(0);
      }
    else //parent process
      {
        waitpid(pid, NULL, WNOHANG);
        printf("I am the parent %d, mypid is %d\n", pid, getpid());
        print_memory(getpid());
        pid = fork();
        if(pid < 0) //fork failed
          {
            fprintf(stderr, "Fork Failed\n");
            return 1;
          }
        else if(pid == 0) //child process
          {  
            printf("I am the third child %d mypid is %d\n", pid, getpid());
            print_memory(getpid());
            exit(0);
          }
        else //parent process
          {
            waitpid(pid, NULL, WNOHANG);
            printf("I am the parent %d, mypid is %d\n", pid, getpid());
            print_memory(getpid());
            
          }
      }
  }
  
  //printf("ram: %i\n", get_memory(getpid()));
  //printf("parent done\n");
  
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
