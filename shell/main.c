#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "util/path.h"

char *BIN = NULL;
char *SHELL_PATH = NULL;

char *HOME = NULL;
char *USER = NULL;
char *HOSTNAME = NULL;

extern int shell(lua_State *L);

int main(int argc, char **argv)
{
    /* Initialize a new Lua state. */

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    /* Set some global variables. */

    // SHELL_DIR = get_exe_dir();
    BIN = (char *)"/bin";
    SHELL_PATH = get_exe_path();

    HOSTNAME = (char *)malloc(HOST_NAME_MAX * sizeof(char));
    memset(HOSTNAME, 0, HOST_NAME_MAX);

    HOME = get_home();
    USER = getlogin();
    gethostname(HOSTNAME, HOST_NAME_MAX);

    lua_pushstring(L, SHELL_PATH);
    lua_setglobal(L, "SHELL_PATH");

    lua_pushstring(L, BIN);
    lua_setglobal(L, "BIN");

    lua_pushstring(L, HOME);
    lua_setglobal(L, "HOME");

    /* Start shell. */

    shell(L);

    /* Free memory allocated for env global variables */

    // free(BIN);
    free(SHELL_PATH);

    free(HOSTNAME);
    
    return 0;
}