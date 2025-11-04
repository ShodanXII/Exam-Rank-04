#include <unistd.h>
#include <stdlib.h>

#define ERR -1

int ft_popen(const char *file, char *const argv[], char type)
{
	int fds[2];
	pid_t pid;

	if (!file || !argv || (type != 'r' && type != 'w') || pipe(fds) == -1)
		return (ERR);
	if ((pid = fork()) == -1)
		return (close(fds[0]), close(fds[1]), ERR);
	if (pid == 0)
	{
		type == 'r' ? dup2(fds[1], 1) : dup2(fds[0], 0);
		close(fds[0]);
		close(fds[1]);
		execvp(file, argv);
		exit(1);
	}
	close(type == 'r' ? fds[1] : fds[0]);
	return(type == 'r' ? fds[0] : fds[1]);
}
