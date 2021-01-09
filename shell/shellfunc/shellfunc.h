#include "context.h"

const void *get_shellf(const char *exec);

int shellf_cd(context *ctx);

int shellf_history_push(context *ctx);

int shellf_history(context *ctx);

int shellf_exec(context *ctx);
