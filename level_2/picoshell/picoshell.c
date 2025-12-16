#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int picoshell(char **cmds[])
{
    int fd[2];
    int pid;
    int prev_fd = -1;
    for (int  i = 0; cmds[i]; i++)
    {
        prev_fd[0] = -1;
        prev_fd[1] = -1;
        
    }
}