#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "shellfunc.h"
#include "env.h"

int shellf_cd(context *ctx)
{
    int retv;
    char *path;

    if (ctx->argv[1] == NULL)
    {
        path = HOME;
    }
    else
    {
        path = ctx->argv[1];
    }

    retv = chdir(path);

    if (retv < 0)
    {
        ctx->err = (char *)malloc(36 * sizeof(char));
        sprintf(ctx->err, "failed to change directory to '%s'.", path);
    }

    return retv;
}