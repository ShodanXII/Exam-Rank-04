#include <stdio.h>

char *g_input;
int g_len;

void sort()
{
	int c;

	for (int i=0; g_input[i]; i++)
	{
		for (int j=i+1; g_input[j]; j++)
		{
			if (g_input[i] > g_input[j])
			{
				c = g_input[i];
				g_input[i] = g_input[j];
				g_input[j] = c;
			}
		}
	}
}

void permute(char *result, int used[], int idx)
{
	if (g_len == idx)
	{
		puts(result);
		return ;
	}
	for (int i=0; i<g_len; i++)
	{
		if (used[i]) continue ;
		result[idx] = g_input[i];
		used[i] = 1;
		permute(result, used, idx + 1);
		used[i] = 0;
	}
}

int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);

	g_input = argv[1];
	sort();
	for (g_len=0; g_input[g_len]; g_len++);
	char result[g_len + 1];
	result[g_len] = '\0';

	int used[g_len];
	for (int i=0; i<g_len; i++) used[i] = 0;

	permute(result, used, 0);
	return (0);
}
