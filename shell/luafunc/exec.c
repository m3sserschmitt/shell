#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "shellfunc.h"
#include "shutil.h"

int exec(lua_State *L)
{
    char *command;
    int stdoutw = 0;

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

    context *c = init_context(L, NULL);

    c->user_input = command;
    c->stdoutw = stdoutw;

    int result = shellf_exec(c);

    if(c->err)
    {
        luaL_error(L, "%s", c->err);
    }

    lua_pushstring(L, c->result);

    free(c->result);
    free(c->err);

    free(c);

    return result < 0 ? 0 : 1;
}
