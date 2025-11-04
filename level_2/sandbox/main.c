#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// prototype
int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

// ANSI colors
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// utilities
static void print_header(const char *title)
{
	printf("\n%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n", CYAN);
	printf("%s▶ TEST:%s %s%s\n", CYAN, RESET, BOLD, title);
	printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", CYAN, RESET);
}

static void print_result(const char *desc, int result, int expected)
{
	const char *status = (result == expected) ? GREEN "✓ PASS" RESET : RED "✗ FAIL" RESET;
	printf("%-60s [%s]\n", desc, status);
}

static void print_line(void)
{
	printf("%s---------------------------------------------------------------%s\n", CYAN, RESET);
}

// dummy functions
void nice_function(void) { return; }

void bad_function_exit_code(void) { exit(1); }

void bad_function_segfault(void)
{
	int *ptr = NULL;
	*ptr = 42;
}

void bad_function_timeout(void)
{
	while (1) {}
}

void bad_function_sleep(void)
{
	sleep(5);
}

int main(void)
{
	int result;

	print_header("Normal function (Nice)");
	result = sandbox(nice_function, 5, true);
	print_result("Expected: exit normally (code 0)", result, 1);
	print_line();

	print_header("Bad function (Exit code 1)");
	result = sandbox(bad_function_exit_code, 5, true);
	print_result("Expected: exit(1)", result, 0);
	print_line();

	print_header("Bad function (Segfault)");
	result = sandbox(bad_function_segfault, 5, true);
	print_result("Expected: segmentation fault detected", result, 0);
	print_line();

	print_header("Bad function (Timeout)");
	result = sandbox(bad_function_timeout, 2, true);
	print_result("Expected: timeout after 2 seconds", result, 0);
	print_line();

	print_header("Bad function (Sleep longer than timeout)");
	result = sandbox(bad_function_sleep, 2, true);
	print_result("Expected: killed after timeout", result, 0);
	print_line();

	printf("\n%sAll tests complete.%s\n", BOLD, RESET);
	return 0;
}
