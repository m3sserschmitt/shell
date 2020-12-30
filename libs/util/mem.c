#include <stdlib.h>

void free_memory(void **mem, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        free(mem[i]);
    }

    free(mem);
}