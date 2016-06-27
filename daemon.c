#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>

#include "daemon.h"

void daemonize(void)
{
	int i, fd0, fd1, fd2;
	struct rlimit rl;

	/* clear file creation mask */
	umask(0);

	/* get maximum number of file descriptors */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		exit(EXIT_FAILURE);

	/* become a session leader to lose controlling TTY */
	setsid();

	/* ensure future opens won't allocate controlling TTYs */
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
		exit(EXIT_FAILURE);

	/*
	 * change the current working directory to the root so
	 * we won't prevent file systems from being unmounted.
	 */
	if (chdir("/") < 0)
		exit(EXIT_FAILURE);

	/* close all open file descriptors */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);

	/* attach file descriptors 0, 1, and 2 to /dev/null */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
		exit(EXIT_FAILURE);
}
