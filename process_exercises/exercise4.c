#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
 pid_t childpid = fork();
 if (childpid == 0) {
  printf("I am child %ld deleting the directory OS.\n", (long)getpid());
  execl("/bin/rm", "rm", "-r", "OS", NULL);
  perror("Deleting directory OS failed!\n");
  exit(1);
 }
 
 if (childpid != wait(NULL)) {
  perror("Parent process failed to wait due to some signal or error.\n");
  exit(1);
 }

 return 0;
}
