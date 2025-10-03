#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid = getpid();
	while (pid > 1)
	{
		char path[64];
		// Write the formatted text "/proc/pid/status" into "path"
		snprintf(path, sizeof(path), "/proc/%d/status", pid);
		/*
		Open the file, which happens to be a text file with the information about the process with the ID "pid" (including the parent process' ID)
		*/
		FILE *f = fopen(path, "r");
		if (!f)
		{
			break;
		}

		char line[256];
		pid_t ppid = -1;
		// Read the file line by line and store each line to "line"
		while (fgets(line, sizeof(line), f))
		{
			// Look for a line with the format "PPid:\t%d", read the integer after it and store that integer in "ppid"
			if (sscanf(line, "PPid:\t%d", &ppid) == 1)
			{
				break;
			}
		}
		fclose(f);

		printf("We are at the process %d, prepare to move onto its parents %d.\n", pid, ppid);
		pid = ppid;
	}
	sleep(10);
	return 0;
}
