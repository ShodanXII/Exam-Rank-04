#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define NICE 1
#define BAD 0
#define ERR -1

void alarm_handler(int sig)
{
	(void)sig;
}

int sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t pid;
	int status;

	if ((pid = fork()) == -1)
		return (ERR);
	if (pid == 0)
	{
		f();
		exit(0);
	}
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
	sa.sa_handler = alarm_handler;
	sigaction(SIGALRM, &sa, NULL);
	alarm(timeout);
	if (waitpid(pid, &status, 0) == -1)
	{
		if (errno == EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, &status, 0);
			if (verbose) printf("Bad function: timed out after %d seconds\n", timeout);
			return (BAD);
		}
		return (ERR);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
	{
		if (verbose)
			printf("Nice function!\n");
		return (NICE);
	}
	if (WIFSIGNALED(status))
		if (verbose) printf("Bad function: %s\n", strsignal(WTERMSIG(status)));
	if (WIFEXITED(status))
		if (verbose) printf("Bad function: exited with code %d\n", WEXITSTATUS(status));
	return (BAD);
}
