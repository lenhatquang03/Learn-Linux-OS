#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
 pid_t childpid = fork();
 if (childpid == 0) {
  printf("I am child %ld, copying directory OS to OS1.\n", (long)getpid());
  /*
   "-r" stands for "--recursively". Without it, "cp" only works on files.
   "-r" allows "cp" to recursively copy all files (included those sub-directories).
  */
  execl("/bin/cp", "cp", "-r", "OS", "OS1", NULL);
  perror("Copying directory OS to OS1 failed!\n");
  exit(1);
 }
 
 /* 
  The child process will not run this code.
  As execl() runs without fails, the parent's codes inherited after execl() will be replaced with the binary code specified in the child process.
  Else, perror() will be executed and the child process exits with status 1.
 */  
 if (childpid != wait(NULL)) {
  printf("I am parent %ld of child %d.\n", (long)getpid(), childpid);
  perror("I failed to wait for my child due to some signal or error.\n");
  exit(1);
 } 

 return 0;
}
