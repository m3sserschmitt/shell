#include <unistd.h>
#include <stdio.h>
#include <wait.h>

#include <lua.h>
#include <lauxlib.h>

#include "shutil.h"
#include "util/str.h"

int create_process(context *ctx)
{
    /* try to fork a new child process. */
    pid_t pid = fork();

    /* return -1 if fork failed. */
    if (pid < 0)
    {
        return -1;
    }
    else if (pid == 0)
    {
        context *previous = ctx->previous;

        /* child closes up reading end of pipe to next child. */
        close(ctx->fds[0]);

        if (previous == NULL)
        {
            /* first child on pipeline duplicates its writing end of pipe
             * to stdout (if so, output is redirected to the next child).
             */
            dup2(ctx->fds[1], 1);
        }
        else
        {
            /* other child proceses close writing end of pipe to previous child. */
            close(previous->fds[1]);

            /* copy reading end of pipe to stdin 
             * to receive output from previous process
             */
            dup2(previous->fds[0], 0);

            /* copy writing end of pipe to stdout
             * so output will be redirected to next process.
             */
            dup2(ctx->fds[1], 1);
        }

        if(ctx->script)
        {
            if(luaL_dofile(ctx->L, ctx->path) != LUA_OK)
            {
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        }

        if (execve(ctx->path, ctx->argv, NULL) < 0)
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        int wstatus;
        if (waitpid(pid, &wstatus, 0) < 0)
        {
            return -1;
        }

        if (WIFEXITED(wstatus))
        {
            int status = WEXITSTATUS(wstatus);

            if (status != EXIT_SUCCESS)
            {
                printf("\nWarning: command '%s' (PID %d) terminated with status code %d.\n", ctx->user_input, pid, status);
                return -1;
            }
        }

        close(ctx->fds[1]);
    }

    return 0;
}

int execute_shellf(context *ctx)
{
    int retv = ctx->shellf_ptr(ctx);
    close(ctx->fds[1]);
    
    return retv;
}

void s_handle_errors(context *ctx)
{
    if(ctx->err)
    {
        printf("%s\n", ctx->err);
    }
}

void handle_errors(lua_State *L, const int ret)
{
    if (ret != LUA_OK)
    {
        printf("Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void free_context(context *ctx)
{
    if (ctx != NULL)
    {
        // free(ctx->user_input);
        free(ctx->path);
        free(ctx->fds);
        free(ctx->result);
        free(ctx->err);

        for (int i = 1; i < ctx->argc; i++)
        {
            free(ctx->argv[i]);
        }

        free(ctx->argv);
    }
}

void free_context_r(context *ctx)
{
    if (ctx != NULL)
    {
        context *previous = (context *)ctx->previous;

        free_context(ctx);

        if (previous != NULL)
        {
            free_context_r(previous);
        }
    }
}
