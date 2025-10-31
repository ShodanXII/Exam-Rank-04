#include <stdio.h>

static char *s, b[1024];
static int n, r, l;

void solve(int i, int j, int o);

void remove_paren(int *x, int i, int j, int o)
{
    if ((*x)--)
    {
        b[j] = ' ';
        solve(i + 1, j + 1, o);
    }
    (*x)++;
}

void keep_paren(char c, int i, int j, int o2)
{
    b[j] = c;
    solve(i + 1, j + 1, o2);
}

void solve(int i, int j, int o)
{
    if (i == n) {
        if (!o && !r && !l)
        {
            b[j] = 0;
            puts(b);
        }
        return ;
    }
    char c = s[i];
    if (c == '(')
    {
        remove_paren(&l, i, j, o);
        keep_paren('(', i, j, o + 1);
    }
    else if (c == ')')
    {
        remove_paren(&r, i, j, o);
        if (o) keep_paren(')', i, j, o - 1);
    }
    else
    {
        b[j] = c;
        solve(i + 1, j + 1, o);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    s = argv[1];
    for (n = 0; s[n]; n++);
    for (int i = 0; i < n; i++) {
        if (s[i] == '(')
            l++;
        else if (s[i] == ')')
            l ? l-- : r++;
    }
    solve(0, 0, 0);
    return 0;
}
