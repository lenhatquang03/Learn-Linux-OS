#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
pid_t childpid;
childpid = fork();
if (childpid == 0) {
 const char *oldname = "OperatingSystems";
 const char *newname = "OS";
 execl("/bin/mv", "mv", oldname, newname, NULL); 
 perror("Change directory's name failed!\n");
 return 1;
} 

// Without this, the child process becomes a zombie.
if (childpid != wait(NULL)) {
 perror("Parent failed to wait due to some signal or error.\n");
 return 1;
}

return 0;
}
