#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>

#include <lualib.h>
#include <lauxlib.h>

#include "env.h"
#include "shutil.h"
#include "shellfunc.h"

#include "util/str.h"
#include "util/path.h"

void print_path()
{
    char *cwd = getcwd(NULL, PATH_MAX);
    char *realative = home_relative(cwd, NULL);

    printf("\033[31;1;24m%s@%s\033[0m:%s$ ", USER, HOSTNAME, realative);

    free(cwd);
    free(realative);
}

ssize_t read_input(char **input)
{
    *input = (char *)malloc(USER_INPUT_SIZE * sizeof(char));

    char *input_buffer = (char *)malloc(USER_INPUT_SIZE * sizeof(char));

    char *endline_ptr = NULL;

    size_t n;
    ssize_t total_read = 0;
    ssize_t read = 0;

    memset(*input, 0, USER_INPUT_SIZE);
    memset(input_buffer, 0, USER_INPUT_SIZE);

    do
    {
        read = getline(&input_buffer, &n, stdin);

        if (read < 0)
        {
            return -1;
        }

        if ((endline_ptr = strstr(input_buffer, "\\")))
        {
            printf("> ... ");

            *endline_ptr = '\n';
            *(endline_ptr + 1) = 0;

            read = endline_ptr - input_buffer + 1;
        }

        strcpy(*input + total_read, input_buffer);

        total_read += read;

    } while (endline_ptr != NULL);

    free(input_buffer);

    strip(*input, "\n");

    return total_read;
}

context *init_context(lua_State *L, context *c)
{
    context *ctx;

    if(c == NULL)
    {
        ctx = (context *)malloc(sizeof(context));
    }
    else 
    {
        ctx = c;
    }

    ctx->L = L;
    ctx->user_input = NULL;

    ctx->argc = 0;
    ctx->argv = NULL;
    ctx->path = NULL;

    ctx->shellf = 0;
    ctx->shellf_ptr = NULL;
    
    ctx->stdoutw = 1;
    ctx->fds = NULL;

    ctx->err = NULL;
    ctx->result = NULL;

    ctx->previous = NULL;

    return ctx;
}

context *parse_input(context *c, int *ctxc)
{
    char **cmd = split(c->user_input, "|", ctxc);
    context *ctx = (context *)malloc(*ctxc * sizeof(context));

    for (int i = 0; i < *ctxc; i++)
    {
        init_context(c->L, &ctx[i]);

        ctx[i].argv = split(cmd[i], " ", &ctx[i].argc);
        ctx[i].user_input = ctx[i].argv[0];
        ctx[i].fds = (int *)malloc(2 * sizeof(int));

        pipe(ctx[i].fds);

        if (!i)
        {
            ctx[i].previous = NULL;
        }
        else
        {
            ctx[i].previous = (void *)(ctx + i - 1);
        }
    }

    return ctx;
}

char *resolve_path(const char *exec, char *path)
{
    char *resolved_path;

    if (path != NULL)
    {
        resolved_path = path;
        *resolved_path = 0;
    }
    else
    {
        resolved_path = (char *)malloc(PATH_MAX * sizeof(char));
        memset(resolved_path, 0, PATH_MAX);
    }

    if (exec[0] == '.' && exec[1] == '/')
    {
        char *cwd = getcwd(NULL, PATH_MAX);

        strcpy(resolved_path, cwd);
        strcat(resolved_path, exec + 1);

        free(cwd);
    }
    else if (strstr(exec, "/"))
    {
        strcpy(resolved_path, exec);
    }
    else
    {
        // lua_getglobal(L, "BIN");
        // const char *bin = luaL_checkstring(L, -1);
        // lua_setglobal(L, "BIN");

        strcpy(resolved_path, BIN);
        strcat(resolved_path, "/");
        strcat(resolved_path, exec);
    }

    return resolved_path;
}

int is_executable(context *ctx)
{
    ctx->shellf = 0;
    ctx->shellf_ptr = NULL;

    ctx->path = NULL;

    if(ctx->user_input == NULL || !strlen(ctx->user_input))
    {
        return 0;
    }

    if (strstr(ctx->user_input, "\n") == NULL)
    {
        char *first_token = (char *)malloc(PATH_MAX * sizeof(char));
        strcpy(first_token, ctx->user_input);

        char *first_space;
        if ((first_space = strstr(first_token, " ")))
        {
            *first_space = 0;
        }

        if((ctx->shellf_ptr = get_shellf(first_token)) != NULL)
        {
            ctx->shellf = 1;

            return 1;
        }

        ctx->path = resolve_path(first_token, NULL);

        struct stat buffer;
        int result = stat(ctx->path, &buffer);

        free(first_token);

        return !result;
    }

    return 0;
}
