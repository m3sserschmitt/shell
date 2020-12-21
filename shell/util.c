#include <string.h>
#include <stdlib.h>

size_t str_count(const char *str_in, const char *str)
{
    size_t count = 0;
    char *ptr = strstr(str_in, str);
    
    while(ptr)
    {
        ptr ++;
        ptr = strstr(ptr, str);
        
        count++;
    }

   return count;
}

char **split(const char *str, const char *delim, long int *count)
{
    size_t total = str_count(str, delim);
    char **tokens = (char **)malloc((total + 1) * sizeof(char *));
    char **token_ptr = tokens;

    char *buffer = (char *)malloc((strlen(str) + 1) * sizeof(char));
    strcpy(buffer, str);

    char *token = strtok(buffer, delim);

    while(token)
    {
        *token_ptr = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(*token_ptr, token);

        token_ptr ++;

        token = strtok(NULL, delim);
    }

    *token_ptr = NULL;

    if(count)
    {
        *count = token_ptr - tokens;
    }

    free(buffer);

    return tokens;
}