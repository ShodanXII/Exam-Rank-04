
#include <stdio.h>
#include <stdlib.h>

int goal;
int n;
int *nbs;
int flag = 0;

void    print_solution(int s[], int idx)
{
    int i;
    for (i=0; i<idx-1; i++)
        printf("%d ", s[i]);
    printf("%d\n", s[i]);
    flag = 1;
}

void    power_set(int s[], int sum, int idx, int pos)
{
    if (sum == goal && idx != 0)
        print_solution(s, idx);
    for (int i=pos; i<n; i++)
    {
        s[idx] = nbs[i];
        sum += nbs[i];
        power_set(s, sum, idx+1, i+1);
        sum -= nbs[i];
    }
}

int main(int argc, char **argv)
{
    if (argc <= 2)
        return (1);
    goal = atoi(argv[1]);
    if (goal == 0)
        printf("\n");
    n = argc-2;
    int s[n];
    int nb[n];
    nbs = nb;
    for (int i=0; i<n; i++)
        nb[i] = atoi(argv[i+2]);
    power_set(s, 0, 0, 0);
    if (!flag && goal != 0)
        printf("\n");
    return (0);
}
