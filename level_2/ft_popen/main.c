#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

/* ------------------ EXTERNAL FUNCTION ------------------ */
int ft_popen(const char *file, char *const argv[], char type);

/* ------------------ COLORS ------------------ */
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define RESET   "\033[0m"
#define GRAY    "\033[90m"

/* ------------------ UI HELPERS ------------------ */
static void print_header(const char *title)
{
    printf("\n%s%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", GRAY, BOLD, RESET);
    printf("%s▶ %s%s%s\n", MAGENTA, CYAN, title, RESET);
    printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n\n", GRAY, RESET);
}

static void print_result(const char *msg, int ok)
{
    printf("   %s[%s]%s %s\n", ok ? GREEN : RED, ok ? "OK" : "FAIL", RESET, msg);
}

static void print_info(const char *msg)
{
    printf("   %s[INFO]%s %s\n", CYAN, RESET, msg);
}

static void print_warn(const char *msg)
{
    printf("   %s[WARN]%s %s\n", YELLOW, RESET, msg);
}

/* ========================================================= */
/*                HELPER FUNCTIONS IMPLEMENTATION             */
/* ========================================================= */

static char *read_fd_into_str(int fd)
{
    char buf[4096];
    ssize_t r;
    size_t len = 0;
    char *result = NULL;

    while ((r = read(fd, buf, sizeof(buf))) > 0)
    {
        char *tmp = realloc(result, len + r + 1);
        if (!tmp)
        {
            free(result);
            return NULL;
        }
        result = tmp;
        memcpy(result + len, buf, r);
        len += r;
    }
    if (result)
        result[len] = '\0';
    return result;
}

static void compare_outputs(const char *cmd, const char *ours)
{
    char tmpfile[] = "/tmp/ftpopen_ref_XXXXXX";
    int fd = mkstemp(tmpfile);
    if (fd == -1)
    {
        perror("mkstemp");
        return;
    }
    close(fd);

    char syscmd[512];
    snprintf(syscmd, sizeof(syscmd), "%s > %s 2>/dev/null", cmd, tmpfile);
    system(syscmd);

    FILE *f = fopen(tmpfile, "r");
    if (!f)
    {
        perror("fopen");
        unlink(tmpfile);
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *ref = malloc(size + 1);
    if (!ref)
    {
        fclose(f);
        unlink(tmpfile);
        return;
    }

    fread(ref, 1, size, f);
    ref[size] = '\0';
    fclose(f);
    unlink(tmpfile);

    if (strcmp(ref, ours) == 0)
        printf("   %s[MATCH]%s Output identical to %s\n", GREEN, RESET, cmd);
    else
        printf("   %s[DIFF]%s Output differs from %s\n", RED, RESET, cmd);

    free(ref);
}

/* ========================================================= */
/*                        TEST CASES                         */
/* ========================================================= */

static void test_read_ls(void)
{
    print_header("TEST 1: ft_popen('ls', 'r')");
    int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
    if (fd == -1)
    {
        print_result("ft_popen('ls') failed", 0);
        fprintf(stderr, "      %s%s%s\n", RED, strerror(errno), RESET);
        return;
    }

    print_info("Reading ft_popen output...");
    char *out = read_fd_into_str(fd);
    close(fd);

    if (!out)
    {
        print_warn("No output received.");
        return;
    }

    printf("%s%s%s\n", GRAY, out, RESET);
    compare_outputs("ls", out);
    free(out);
}

static void test_pipe_ls_grep(void)
{
    print_header("TEST 2: Simulated pipe 'ls | grep c'");
    int fd_ls = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');
    if (fd_ls == -1)
    {
        print_result("ft_popen('ls') failed", 0);
        return;
    }

    dup2(fd_ls, 0);
    int fd_grep = ft_popen("grep", (char *const[]){"grep", "c", NULL}, 'r');
    if (fd_grep == -1)
    {
        print_result("ft_popen('grep') failed", 0);
        close(fd_ls);
        return;
    }

    char *out = read_fd_into_str(fd_grep);
    close(fd_ls);
    close(fd_grep);

    if (!out)
    {
        print_warn("grep returned no output.");
        return;
    }

    printf("%s%s%s\n", GRAY, out, RESET);
    compare_outputs("ls | grep c", out);
    free(out);
}

static void test_invalid_args(void)
{
    print_header("TEST 3: Invalid arguments");
    int fd;

    fd = ft_popen(NULL, (char *const[]){"ls", NULL}, 'r');
    print_result("NULL file", fd == -1);

    fd = ft_popen("ls", NULL, 'r');
    print_result("NULL argv", fd == -1);

    fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'x');
    print_result("Invalid mode", fd == -1);
}

static void test_write_mode(void)
{
    print_header("TEST 4: Write mode to 'cat'");
    int fd = ft_popen("cat", (char *const[]){"cat", NULL}, 'w');
    if (fd == -1)
    {
        print_result("ft_popen('cat') failed", 0);
        fprintf(stderr, "      %s%s%s\n", RED, strerror(errno), RESET);
        return;
    }
    close(fd);
    print_result("Data written successfully", 1);
}

/* ========================================================= */
/*                        MAIN TEST SUITE                    */
/* ========================================================= */

int main(void)
{
    printf("\n%s%sFT_POPEN TEST SUITE%s\n", BOLD, BLUE, RESET);
    printf("%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", GRAY, RESET);

    test_read_ls();
    test_pipe_ls_grep();
    test_invalid_args();
    test_write_mode();

    printf("\n%s━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━%s\n", GRAY, RESET);
    printf("%s%sAll tests finished.%s\n\n", BOLD, GREEN, RESET);
    return 0;
}
