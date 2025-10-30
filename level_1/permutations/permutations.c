#include <stdio.h>

int n;
char *arg;

void    sort_arr(void)
{
    char tmp;
    for (int i=0; arg[i]; i++)
    {
        for (int j=i+1; arg[j]; j++)
        {
            if (arg[i] > arg[j])
            {
                tmp = arg[i];
                arg[i] = arg[j];
                arg[j] = tmp;
            }
        }
    }
}

int ft_strlen()
{
    int i=0;
    while (arg[i])
        i++;
    return (i);
}

void permute(char *str, int idx, int used[])
{
    if (n == idx)
    {
        puts(str);
        return ;
    }
    for (int i=0; i<n; i++)
    {
        if (used[i]) continue ;
        str[idx] = arg[i];
        used[i] = 1;
        permute(str, idx + 1, used);
        used[i] = 0;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return (1);
    arg = argv[1];
    n = ft_strlen();
    int used[n];
    for (int i=0; i < n; i++)
        used[i] = 0;
    char str[n + 1];
    str[n] = '\0';
    sort_arr();
    permute(str, 0, used);
    return (0);
}
