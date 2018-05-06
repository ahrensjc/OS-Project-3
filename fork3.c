#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>

int GetRamInKB(void)
{
    FILE *meminfo = fopen("/proc/meminfo", "r");
    if(meminfo == NULL)
    {
      exit(1);
    } 

    char line[256];
    int ram = -1;
    while(fgets(line, sizeof(line), meminfo))
    {
        //printf(line);
        if(sscanf(line, "MemFree: %d kB", &ram) == 1)
        {
            printf("Free memory: %d (kB)\n", ram);
            fclose(meminfo); 
            return ram;           
        }
    }

    fclose(meminfo);
    return ram;
}

int main(int argc, char* argv[])
{
  pid_t pid;
  
  //fork a child process
  GetRamInKB();
  
  int bigArray[1000][1000];
  pid = fork();
  GetRamInKB();
  
  if(pid < 0) //fork failed
  {
    fprintf(stderr, "Fork Failed\n");
    return 1;
  }
  else if(pid == 0) //child process
  {
    //printf("Even numbers:\n");
    
    
    //printf("There were %d even numbers.\n", evens);
  
    printf("I am the child %d mypid is %d\n", pid, getpid());
    //execlp("/bin/ps", "ps", "-al", NULL);
    //printf("SHOULD NOT PRINT OUT");
    //char * args[3];
    //args[0] = "./hello";
    //args[1] ="5";
    //args[2] = "text";
    //args[3] = NULL;
    //execv(args[0], args);
    //printf("SHOULD NOT PRINT OUT");
  }
  else //parent process
  {
    wait(NULL);
    //printf("Numbers divisable by 3:\n");
    
    //printf("There were %d numbers divisable by 3.\n", divThree);
  
    printf("I am the parent %d, mypid is %d\n", pid, getpid());
    
    
    //printf("Child Complete\n");
  }
  
  return 0;
}