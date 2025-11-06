
#include <stdio.h>
#include <stdlib.h>

int g_goal;
int g_len;
int *g_input;
int g_flag = 0;

void    print_solution(int *result, int idx)
{
    int i;
    for (i=0; i<idx-1; i++)
        printf("%d ", result[i]);
    printf("%d\n", result[i]);
    g_flag = 1;
}

void    power_set(int *result, int sum, int idx, int pos)
{
    if (sum == g_goal && idx != 0)
        print_solution(result, idx);
    for (int i=pos; i<g_len; i++)
    {
        result[idx] = g_input[i];
        sum += g_input[i];
        power_set(result, sum, idx + 1, i + 1);
        sum -= g_input[i];
    }
}

int main(int argc, char **argv)
{
    if (argc <= 2)
        return (1);
    g_goal = atoi(argv[1]);
    if (g_goal == 0)
        printf("\n");

    g_len = argc-2;
    int result[g_len];
    int nb[g_len];
    g_input = nb;
    for (int i=0; i<g_len; i++)
        g_input[i] = atoi(argv[i+2]);

    power_set(result, 0, 0, 0);
    if (!g_flag && g_goal != 0)
        printf("\n");
    return (0);
}
