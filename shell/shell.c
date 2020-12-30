#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "shutil.h"
#include "env.h"

extern int cd(lua_State *L);
extern int exec(lua_State *L);

int execute(lua_State *L, const char *user_input)
{
    int retv;

    if (is_executable(L, user_input))
    {
        lua_getglobal(L, "exec");
        lua_pushstring(L, user_input);
        lua_pushinteger(L, 1);

        retv = lua_pcall(L, 2, 1, 0);
    }
    else
    {
        retv = luaL_dostring(L, user_input);
    }

    handle_errors(L, retv);

    return retv;
}

int shell(lua_State *L)
{
    /* Set some C functions for basic operations into current Lua state. */
    lua_pushcfunction(L, cd);
    lua_setglobal(L, "cd");

    lua_pushcfunction(L, exec);
    lua_setglobal(L, "exec");

    char *user_input;

    /* start command line loop. */
    while (1)
    {
        print_path();

        if (read_input(&user_input) < 0)
        {
            return -1;
        }

        if (!strncmp(user_input, "exit", 4))
        {
            break;
        }

        execute(L, user_input);

        lua_settop(L, 0);
        free(user_input);
    }

    free(user_input);

    return 0;
}
