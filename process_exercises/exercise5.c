#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
 // Show the source process' ID
 printf("The source process is %ld.\n", (long)getppid());
 pid_t childpid = 0;
 // Create a chain process of length 5
 for (int i = 0; i < 4; i++) {
  if (childpid = fork()) {
   printf("I am process %ld, my child is %d.\n", (long)getpid(), childpid);
   break; 
  }
 }
 wait(NULL);
 return 0;
}
