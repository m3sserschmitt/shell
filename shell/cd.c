#include <unistd.h>

#include <lua.h>
#include <lauxlib.h>

int cd(lua_State *L)
{
    int stack_size = lua_gettop(L);

    if(!stack_size)
    {
        lua_getglobal(L, "HOME");
    }

    const char *path = luaL_checkstring(L, 1);

    return chdir(path) < 0 ? -1 : 0;
}