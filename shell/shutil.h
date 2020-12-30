#include <stdlib.h>

#include <lua.h>

#define USER_INPUT_SIZE 4096

ssize_t read_input(char **input);

void handle_errors(lua_State *L, const int ret);

void print_path();

char *resolve_path(lua_State *L, const char *exec, char *path);

int is_executable(lua_State *L, const char *exec);
