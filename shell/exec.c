#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define RECV_BUFFER 1024;

extern size_t str_count(const char *in, const char *str);
extern char **split(const char *str, const char *delim, long int *count);

int create_process(const char *exec, char *const *argv, int **fds, long i)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        return -1;
    }
    else if (pid == 0)
    {
        close(fds[i][0]);

        if (i == 0)
        {
            dup2(fds[i][1], 1);
        }
        else
        {
            close(fds[i - 1][1]);

            dup2(fds[i - 1][0], 0);
            dup2(fds[i][1], 1);
        }

        return execve(exec, argv, NULL);
    }
    else
    {
        wait(NULL);
        close(fds[i][1]);
    }

    return 0;
}

int exec(lua_State *L)
{
    const char *cmd = luaL_checkstring(L, 1);

    long count;
    char **commands = split(cmd, "|", &count);

    int **fds = (int **)malloc((count) * sizeof(int *));

    for (long i = 0; i < count; i++)
    {
        long argc;
        char **argv = split(commands[i], " ", &argc);

        fds[i] = (int *)malloc(2 * sizeof(int));
        pipe(fds[i]);

        if (create_process(argv[0], argv, fds, i) < 0)
        {
            return -1;
        }

        for (long j = 0; j < argc; j++)
        {
            free(argv[j]);
        }

        free(argv);
        free(commands[i]);
    }

    free(commands);

    size_t size = RECV_BUFFER;
    char *recv_buffer = (char *)malloc(size * sizeof(char));
    memset(recv_buffer, 0, size);

    char *recv = (char *)malloc(512 * sizeof(char));
    memset(recv, 0, 512);

    while (read(fds[count - 1][0], recv, 512) > 0)
    {
        if (strlen(recv_buffer) + strlen(recv) > size)
        {
            size += size;
            recv_buffer = realloc(recv_buffer, size * sizeof(char));
        }

        strcat(recv_buffer, recv);
        memset(recv, 0, 512);
    }

    for (long i = 0; i < count; i++)
    {
        free(fds[i]);
    }

    free(fds);

    lua_pushstring(L, recv_buffer);

    free(recv);
    free(recv_buffer);

    return 1;
}
