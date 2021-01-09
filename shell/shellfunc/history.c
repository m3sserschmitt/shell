#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shellfunc.h"

#define HISTORY_MAX 2048

static char *history_buffer[HISTORY_MAX];
static size_t history_counter = 0;

int shellf_history_push(context *ctx)
{
    const char *in = ctx->user_input;
    size_t inlen = strlen(in);

    history_buffer[history_counter] = (char *)malloc((inlen + 1) * sizeof(char));

    strcpy(history_buffer[history_counter++], in);

    return 0;
}

int shellf_history(context *ctx)
{
    // close(ctx->fds[0]);
    for (size_t i = 0; i < history_counter; i++)
    {
        dprintf(ctx->fds[1], "%zi\t%s\n", i, history_buffer[i]);
        // printf("%zi\t%s\n", i, history_buffer[i]);
    }

    return 0;
}