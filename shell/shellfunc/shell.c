#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <unistd.h>

#include "context.h"
#include "shellfunc.h"
#include "luafunc.h"
#include "shutil.h"
#include "env.h"

int execute(context *ctx)
{
    int retv;

    // if(ctx->err)
    // {
    //     free(ctx->err);
    //     ctx->err = NULL;
    // }

    // if(ctx->result)
    // {
    //     free(ctx->result);
    //     ctx->result = NULL;
    // }

    int is_exec = is_executable(ctx);

    // printf("%d, %d\n", is_exec, ctx->script);

    /*if(ctx->script)
    {
        retv = luaL_dofile(ctx->L, ctx->path);
        handle_errors(ctx->L, retv);
    }
    else */if (is_exec)
    {
        retv = shellf_exec(ctx);
        s_handle_errors(ctx);
    }
    else
    {
        retv = luaL_dostring(ctx->L, ctx->user_input);
        handle_errors(ctx->L, retv);
    }

    return retv;
}

int shell(lua_State *L)
{
    chdir(HOME);
    
    /* Set some C functions for basic operations into current Lua state. */
    lua_pushcfunction(L, cd);
    lua_setglobal(L, "cd");

    lua_pushcfunction(L, exec);
    lua_setglobal(L, "exec");
    
    char *user_input;

    /* start command line loop. */
    while (1)
    {
        context *ctx = init_context(L, NULL);

        print_path();

        if (read_input(&user_input) < 0)
        {
            return -1;
        }

        if (!strncmp(user_input, "exit", 4))
        {
            break;
        }

        ctx->user_input = user_input;

        execute(ctx);
        shellf_history_push(ctx);
        
        lua_settop(L, 0);

        free(user_input);
        free_context(ctx);
    }

    return 0;
}
