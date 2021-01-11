#include "shellfunc.h"

#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "shutil.h"

#include "util/str.h"
#include "util/mem.h"
#include "util/data.h"


int shellf_exec(context *ctx)
{
    int ctxc;
    context *c_ctx = parse_input(ctx, &ctxc);
    context *ctx_ptr;

    if (!ctxc)
    {
        return 0;
    }

    for (int i = 0; i < ctxc; i++)
    {
        ctx_ptr = c_ctx + i;

        if (!is_executable(ctx_ptr))
        {
            ctx->err = (char *)malloc((16 + strlen(ctx_ptr->user_input)) * sizeof(char));
            sprintf(ctx->err, "'%s' not found.", ctx_ptr->user_input);

            return -1;
        }

        /*if (ctx_ptr->script)
        {
            if(execute_script(ctx_ptr) < 0)
            {

            }
        }
        else */if (ctx_ptr->shellf)
        {
            if(execute_shellf(ctx_ptr) < 0)
            {
                ctx->err = (char *)malloc((strlen(ctx_ptr->err) * sizeof(char)));
                strcpy(ctx->err, ctx_ptr->err);

                free_context_r(c_ctx + ctxc - 1);

                return -1;
            }
        }
        else if (create_process(ctx_ptr) < 0)
        {
            ctx->err = (char *)malloc((54 + strlen(ctx_ptr->user_input)) * sizeof(char));
            sprintf(ctx->err, "errors were encountered while executing command '%s'.", ctx_ptr->user_input);

            free_context_r(c_ctx + ctxc - 1);

            return -1;
        }
    }

    ssize_t bytes_read = read_data(ctx_ptr->fds[0], &ctx->result, ctx->stdoutw);
    free_context_r(ctx_ptr);

    if (bytes_read < 0)
    {
        ctx->err = (char *)malloc(50 * sizeof(char));
        strcpy(ctx->err, "failed to receive output data from child process.");

        return -1;
    }

    if (bytes_read && ctx->result[bytes_read - 1] == '\n')
    {
        ctx->result[bytes_read - 1] = 0;
    }

    return 0;
}
