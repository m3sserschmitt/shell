#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>

#include <lua.h>
#include <lauxlib.h>

#include "shutil.h"

#include "util/str.h"
#include "util/mem.h"
#include "util/data.h"

/**
 * @brief Start a new process
 * 
 * @param exec Path of executable
 * @param argv Null terminated list of arguments.
 * @param fds Pipe descriptors for interprocess comunication.
 * @param i Process index onto pipeline
 * @return int 0, if successful, -1 for errors.
 */
int create_process(const char *exec, char *const *argv, int **fds, int i)
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
        /* child closes up reading end of pipe to next child. */
        close(fds[i][0]);

        if (i == 0)
        {
            /* first child on pipeline duplicates its writing end of pipe
             * to stdout (if so, output is redirected to the next child).
             */
            dup2(fds[i][1], 1);
        }
        else
        {
            /* other child proceses close writing end of pipe to previous child. */
            close(fds[i - 1][1]);

            /* copy reading end of pipe to stdin 
             * to receive output from previous process
             */
            dup2(fds[i - 1][0], 0);

            /* copy writing end of pipe to stdout
             * so output will be redirected to next process.
             */
            dup2(fds[i][1], 1);
        }
        
        if (execve(exec, argv, NULL) < 0)
        {
            return EXIT_FAILURE;
        }
    }
    else
    {
        int wstatus;
        if(waitpid(pid, &wstatus, 0) < 0)
        {
            return -1;
        }

        if(WIFEXITED(wstatus))
        {
            int status = WEXITSTATUS(wstatus);

            if(status != EXIT_SUCCESS)
            {
                printf("\nWarning: command '%s' (PID %d) terminated with status code %d.\n", exec, pid, status);
                return -1;
            }
        }

        close(fds[i][1]);
    }

    return 0;
}

/**
 * @brief Execute command and the result is pushed back on stack.
 * 
 * @param L Current Lua state.
 * @return int number of values pushed back on stack or -1 is errors occured.
 */
int exec(lua_State *L)
{
    /* command to be executed. */
    char *command;

    /* if 1, write command output to stdout (1, by default), 0 otherwise. */
    int stdoutw = 1;

    /* check number of arguments. */
    switch (lua_gettop(L))
    {
    case 0:
        /* if no argument passed, then return. */
        luaL_error(L, "no arguments provided to function call.");

    case 1:
        /* get first argument from stack, command to be executed. */
        command = (char *)luaL_checkstring(L, 1);
        break;

    default:
        /* get command to be executed and stdoutw */
        command = (char *)luaL_checkstring(L, 1);
        stdoutw = luaL_checkinteger(L, 2);
        break;
    }

    /* check if command is a valid string. */
    if (command == NULL)
    {
        luaL_error(L, "argument 0 should be of type string.");
    }

    /* number of subcommands (if pipes were used) */
    int cmdc;
    char **cmd = split(command, "|", &cmdc);

    /* allocate memory for pipe descriptors */
    int **fds = (int **)malloc((cmdc) * sizeof(int *));

    /* iterate over all subcommands. */
    for (int i = 0; i < cmdc; i++)
    {
        /* arguments count for subcommand. */
        int argc;

        /* subcommand arguments. */
        char **argv = split(cmd[i], " ", &argc);

        /* create pipe for interprocess comunication. */
        fds[i] = (int *)malloc(2 * sizeof(int));
        pipe(fds[i]);

        /* try to create process and return error if it fails. */
        
        char *exec_cmd = resolve_path(L, argv[0], NULL);
        if (create_process(exec_cmd, argv, fds, i) < 0)
        {
            luaL_error(L, "errors were encountered while executing command '%s'.", argv[0]);
        }
        free(exec_cmd);

        /* free command string and its arguments. */
        free_memory((void **)argv, argc);
        free(cmd[i]);
    }

    /* free command vector. */
    free(cmd);

    /* buffer to receive results. */
    char *recv_buffer;

    /* read output from command. */
    ssize_t bytes_read = read_data(fds[cmdc - 1][0], &recv_buffer, stdoutw);

    /* release memory for pipe descriptors after output was received. */
    free_memory((void **)fds, cmdc);

    /* check for errors */
    if (bytes_read < 0)
    {
        luaL_error(L, "failed to receive output data from child process.");
    }

    /* remove trailing new line char */
    if (bytes_read && recv_buffer[bytes_read - 1] == '\n')
    {
        recv_buffer[bytes_read - 1] = 0;
    }

    /* push result on stack. */
    lua_pushstring(L, recv_buffer);
    free(recv_buffer);

    /* return number of results;
     * return 1, since only one string was pushed back on stack.
     */
    return 1;
}
