
#include <stdio.h>
#include <stdlib.h>

int g_len;
int *g_board;

void print_solution()
{
    int i;
    for (i=0; i<g_len-1; i++)
        fprintf(stdout, "%d ", g_board[i]);
    fprintf(stdout, "%d\n", g_board[i]);
}

int is_valid(int raw, int col)
{
    int q_col, q_raw;
    for (int i=0; i<raw; i++)
    {
        q_raw = i, q_col = g_board[i];
        if (q_col == col)
            return (-1);
        else if (q_raw - q_col == raw - col)
            return (-1);
        else if (q_raw + q_col == raw + col)
            return (-1);
    }
    return (0);
}

void    n_queens(int raw)
{
    if (raw == g_len)
    {
        print_solution();
        return ;
    }
    for (int col=0; col<g_len; col++)
    {
        if (is_valid(raw, col) == 0)
        {
            g_board[raw] = col;
            n_queens(raw+1);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    g_len = atoi(argv[1]);
	if (g_len == 0)
		return (fprintf(stdout, "\n"), 1);
    int board[g_len];
    g_board = board;
    n_queens(0);
    return 0;
}
