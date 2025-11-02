
#include <stdio.h>

int must_fix = 0;
char *input;

int count_unmatched_parens()
{
	int unopened = 0, unclosed = 0;
	for (int i=0; input[i]; i++)
	{
		if (input[i] == '(') unclosed++;
		else if (input[i] == ')')
		{
			if (unclosed > 0) unclosed--;
			else unopened++;
		}
	}
	return (unclosed + unopened);
}

void rip(int changes_nbr, int started_pos)
{
	if (changes_nbr > must_fix) return ;
	if (changes_nbr == must_fix && count_unmatched_parens() == 0)
	{
		puts(input);
		return ;
	}
	int c;
	for (int i=started_pos; input[i]; i++)
	{
		if (input[i] == '(' || input[i] == ')')
		{
			c = input[i];
			input[i] = ' ';
			rip(changes_nbr + 1, i + 1);
			input[i] = c;
		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) return (1);
	input = argv[1];
	must_fix = count_unmatched_parens();
	rip(0, 0);
}