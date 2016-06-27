#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "version.h"
#include "daemon.h"

int main(int argc, char *argv[])
{
	if (argc == 2) {
		if (strcmp(argv[1], "-v") == 0)
			fprintf(stdout, "%s\n", VERSION);
		return 0;
	}

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
	} else if (pid == 0) {	/* child */
		daemonize();
		for (;;) {
			sleep(3);
		}
	} else {	/* parent */
		exit(0);
	}

	return 0;
}
