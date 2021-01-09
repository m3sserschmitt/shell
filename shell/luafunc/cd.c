#include <unistd.h>
#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>

#include "shellfunc.h"

int cd(lua_State *L)
{
    int stack_size = lua_gettop(L);

    if(!stack_size)
    {
        lua_getglobal(L, "HOME");
    }

    context *ctx = (context *)malloc(sizeof(context));

    ctx->user_input = luaL_checkstring(L, 1);

    return shellf_cd(ctx);
}
