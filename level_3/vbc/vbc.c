#include <stdio.h>
#include <ctype.h>

int add();

char *g_input;
int g_error = 0;

void    unexpected(char c)
{
	g_error = 1;
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n");
}

int peek() {return (*g_input); }

int consume() {return (*g_input++); }

int factory()
{
	if (isdigit(peek()))
		return (consume() - '0');
	else if (peek() == '(')
	{
		consume();
		int val = add();
		if (peek() != ')') return (unexpected(peek()), 1);
		consume();
		return (val);
	}
	return (unexpected(peek()), 1);
}

int multiple()
{
	int val = factory();
	if (g_error) return (1);
	while (peek() == '*')
	{
		consume();
		val *= factory();
		if (g_error) return (1);
	}
	return (val);
}

int add()
{
	int val = multiple();
	if (g_error) return (1);
	while (peek() == '+')
	{
		consume();
		val += multiple();
		if (g_error) return (1);
	}
	return (val);
}

int main(int argc, char **argv)
{
	if (argc != 2) return (1);
	g_input = argv[1];
	int res = add();
	if (g_error) return (1);
	if (peek() != '\0') return (unexpected(peek()), 1);
	printf("%d\n", res);
	return (0);
}
