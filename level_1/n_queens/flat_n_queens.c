
#include <stdio.h>
#include <stdlib.h>

int n;
int *board_g;

void print_solution()
{
    int i;
    for (i=0; i<n-1; i++)
        fprintf(stdout, "%d ", board_g[i]);
    fprintf(stdout, "%d\n", board_g[i]);
}

int is_valid(int raw, int col)
{
    int q_col, q_raw;
    for (int i=0; i<raw; i++)
    {
        q_raw = i, q_col = board_g[i];
        if (q_col == col)
            return (0);
        else if (q_raw - q_col == raw - col)
            return (0);
        else if (q_raw + q_col == raw + col)
            return (0);
    }
    return (1);
}

void    n_queens(int raw)
{
    if (raw == n)
    {
        print_solution();
        return ;
    }
    for (int col=0; col<n; col++)
    {
        if (is_valid(raw, col))
        {
            board_g[raw] = col;
            n_queens(raw+1);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2 || atoi(argv[1]) <= 0)
        return (1);
    n = atoi(argv[1]);
    int board[n];
    board_g = board;
    n_queens(0);
    return 0;
}
