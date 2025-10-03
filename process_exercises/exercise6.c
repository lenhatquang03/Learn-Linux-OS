#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
 printf("The source process is %ld.\n", (long)getppid());
 pid_t childpid;
 for (int i = 0; i < 5; i++) {
  if ((childpid = fork()) == 0) { break; }
   wait(NULL); 
   printf("I am process %ld, my child is %d.\n", (long)getpid(), childpid);
 }
 return 0;
}
