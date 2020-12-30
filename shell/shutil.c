#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>

#include <lualib.h>
#include <lauxlib.h>

#include "env.h"
#include "shutil.h"

#include "util/str.h"
#include "util/path.h"

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

void handle_errors(lua_State *L, const int ret)
{
    if (ret != LUA_OK)
    {
        printf("Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

void print_path()
{
    char *cwd = getcwd(NULL, PATH_MAX);
    char *realtive = (char *)malloc(strlen(cwd) * sizeof(char));

    home_relative(cwd, realtive);

    printf("%s@%s:%s$ ", USER, HOSTNAME, realtive);

    free(cwd);
}

char *resolve_path(lua_State *L, const char *exec, char *path)
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
        lua_getglobal(L, "BIN");
        const char *bin = luaL_checkstring(L, -1);

        strcpy(resolved_path, bin);
        strcat(resolved_path, "/");
        strcat(resolved_path, exec);
    }

    return resolved_path;
}

int is_executable(lua_State *L, const char *exec)
{
    if (strstr(exec, "\n") == NULL)
    {
        char *path = (char *)malloc(PATH_MAX * sizeof(char));
        strcpy(path, exec);

        char *first_space;
        if ((first_space = strstr(path, " ")))
        {
            *first_space = 0;
        }

        char *resolved_path = resolve_path(L, path, NULL);

        struct stat buffer;
        int result = stat(resolved_path, &buffer);

        free(resolved_path);
        free(path);

        return !result;
    }

    return 0;
}