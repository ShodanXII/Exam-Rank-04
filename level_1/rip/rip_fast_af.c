#include <stdio.h>

static char *input;
static char buffer[1024];
static int input_length;
static int unmatched_close, unmatched_open;

void solve(int input_idx, int buffer_idx, int open_count);

void try_remove_paren(int *remaining_to_remove, int input_idx, int buffer_idx, int open_count)
{
    if ((*remaining_to_remove)--)
    {
        buffer[buffer_idx] = ' ';
        solve(input_idx + 1, buffer_idx + 1, open_count);
    }
    (*remaining_to_remove)++;
}

void keep_paren(char c, int input_idx, int buffer_idx, int open_count)
{
    buffer[buffer_idx] = c;
    solve(input_idx + 1, buffer_idx + 1, open_count);
}

void solve(int input_idx, int buffer_idx, int open_count)
{
    if (input_idx == input_length)
    {
        if (!open_count && !unmatched_close && !unmatched_open)
        {
            buffer[buffer_idx] = 0;
            puts(buffer);
        }
        return;
    }
    char c = input[input_idx];
    if (c == '(')
    {
        try_remove_paren(&unmatched_open, input_idx, buffer_idx, open_count);
        keep_paren('(', input_idx, buffer_idx, open_count + 1);
    }
    else if (c == ')')
    {
        try_remove_paren(&unmatched_close, input_idx, buffer_idx, open_count);
        if (open_count) keep_paren(')', input_idx, buffer_idx, open_count - 1);
    }
    else
    {
        buffer[buffer_idx] = c;
        solve(input_idx + 1, buffer_idx + 1, open_count);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
        return 1;
    input = argv[1];
    for (input_length = 0; input[input_length]; input_length++);
    for (int i = 0; i < input_length; i++)
    {
        if (input[i] == '(')
            unmatched_open++;
        else if (input[i] == ')')
            unmatched_open ? unmatched_open-- : unmatched_close++;
    }
    solve(0, 0, 0);
    return 0;
}
