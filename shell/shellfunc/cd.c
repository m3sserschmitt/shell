#include <unistd.h>

#include "shellfunc.h"
#include "env.h"

int shellf_cd(context *ctx)
{
    if(ctx->argv[1] == NULL)
    {
        return chdir(HOME);
    }

    return chdir(ctx->argv[1]);
}