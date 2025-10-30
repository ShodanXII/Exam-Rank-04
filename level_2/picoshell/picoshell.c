#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int	picoshell(char **cmds[])
{
	int fds[2];
	int prev_fd = -1;

	for (int i = 0; cmds[i]; i++)
	{
		fds[0] = -1;
		fds[1] = -1;
		if (cmds[i + 1] && pipe(fds) == -1)
			return (close(prev_fd), 1);
		switch (fork())
		{
			case -1:
				return (close(fds[0]), close(fds[1]), close(prev_fd), 1);
			case 0:
			{
				if (cmds[i + 1]) dup2(fds[1], 1);
				if (i)	dup2(prev_fd, 0);
				close(prev_fd);
				close(fds[0]);
				close(fds[1]);
				execvp(cmds[i][0], cmds[i]);
				exit(1);
			}
			default:
			{
				close(fds[1]);
				close(prev_fd);
				prev_fd = fds[0];
			}
		}
	}
	close(prev_fd);
	while (wait(NULL) != -1);
	return (0);
}
