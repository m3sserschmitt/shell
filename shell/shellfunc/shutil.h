#include <stdlib.h>

#include <lua.h>

#include "context.h"

#define USER_INPUT_SIZE 4096

void print_path();

ssize_t read_input(char **input);

context *init_context(lua_State *L, context *ctx);

context *parse_input(context *ctx, int *ctxc);

char *resolve_path(const char *exec, char *path);

int is_executable(context *ctx);

int create_process(context *ctx);

int execute_shellf(context *ctx);

void s_handle_errors(context *ctx);

void handle_errors(lua_State *L, const int ret);

void free_context(context *ctx);

void free_context_r(context *ctx);
