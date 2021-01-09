#include <string.h>

#include "shellfunc.h"

static const char *func_names[] = {"cd", "history", NULL};
static const void (*func_addr[]) = {shellf_cd, shellf_history, NULL};

const void *get_shellf(const char *exec)
{
    const char **func_name = func_names;
    const void (**func_ptr) = func_addr;

    while (*func_name)
    {
        if (!strcmp(exec, *func_name))
        {
            return *func_ptr;
        }

        func_ptr ++;
        func_name ++;
    }

    return NULL;
}
