#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
pid_t childpid;
childpid = fork();
if (childpid == 0) {
 const char *dirname = "OperatingSystems";
 if (mkdir(dirname, 0755) == 0) {
  printf("Directory %s is created successfully!\n", dirname);
 } else {
  perror("Create directory failed!\n");
  return 1; 
 }
} else {
  // Without this, the child process becomes a zombie.
  wait(NULL);
}
return 0;
}
