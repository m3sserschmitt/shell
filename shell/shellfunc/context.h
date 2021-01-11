#include <lua.h>

#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct 
{
    // current Lua State
    lua_State *L; 

    // user command
    const char *user_input;
    
    // arguments count
    int argc;

    // values for command arguments
    char **argv;

    // path to executable
    char *path;

    // 1 if user command is a shell function, 0 in case of external commands
    int shellf;

    // pointer to shell function to be executed
    int (*shellf_ptr)(void *);

    // 1 if provided cmd is a file containing a script, 0 otherwise.
    int script;

    // 1 to print output to stdout, 0 otherwise
    int stdoutw;

    // descriptors to read / write data: 0 for reading and 1 for writing (pipes, for example)
    int *fds;

    // if errors occured, it contains error message
    char *err;

    // command output
    char *result;

    // pointer to previous context 
    // (in case of pipes, it is useful to kwnow context in which previous command was executed)
    void *previous;

} context;

#endif