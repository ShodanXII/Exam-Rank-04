#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int picoshell(char **cmds[])
{
    if(!cmds || !*cmds)
        return 1;
    int fd[2];
    int prev_fd = -1;
    for (int i = 0; cmds[i]; i++)
    {
        fd[0] = -1;
        fd[1] = -1;
        if(cmds[i + 1] && pipe(fd) == -1)
        {
            close(prev_fd);
            return 1;
        }
        int pid = fork();
        if(pid == -1)
        {
            close(fd[0]);
            close(fd[1]);
            close(prev_fd);
            return 1;
        }
        else if(pid == 0)
        {
            if(cmds[i + 1])
                dup2(fd[1], 1);
            if(i)
                dup2(prev_fd, 0);
            close(fd[0]);
            close(fd[1]);
            close(prev_fd);
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        else
        {
            prev_fd = fd[0];
            close(fd[0]);
            close(fd[1]);
        }
    }
    close(prev_fd);
    while(wait(NULL) != -1)
        ;
    return 0;
}