#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define ERR -1

char    *gnl(int fd)
{
    static char buffer[100];
    char        line[70000];
    static int  b_pos;
    static int  b_read;
    int         i;

    i = 0;
    if (fd < 0 || 100 < 0)
        return (NULL);
    while (1)
    {
        if (b_pos >= b_read)
        {
            b_pos = 0;
            b_read = read(fd, buffer, 100);
            if (b_read <= 0)
                break ;
        }
        line[i++] = buffer[b_pos++];
        if (buffer[b_pos - 1] == '\n')
            break ;
    }
    line[i] = '\0';
    if (i == 0)
        return (NULL);
    return (strdup(line));
}

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

int main()
{
    int  fd;
    char *line;

    fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');
    while ((line = gnl(fd)))
        puts(line);
    return (0);
}