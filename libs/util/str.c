#include <string.h>
#include <stdlib.h>

size_t str_count(const char *str_in, const char *str)
{
    size_t count = 0;
    char *ptr = strstr(str_in, str);

    while (ptr)
    {
        ptr++;
        ptr = strstr(ptr, str);

        count++;
    }

    return count;
}

char **split(const char *str, const char *delim, int *count)
{
    size_t total = str_count(str, delim);
    char **tokens = (char **)malloc((total + 2) * sizeof(char *));
    char **token_ptr = tokens;

    char *buffer = (char *)malloc((strlen(str) + 1) * sizeof(char));
    strcpy(buffer, str);

    char *token = strtok(buffer, delim);

    while (token)
    {
        *token_ptr = (char *)malloc((strlen(token) + 1) * sizeof(char));
        strcpy(*token_ptr, token);

        token_ptr++;

        token = strtok(NULL, delim);
    }

    *token_ptr = NULL;

    if (count)
    {
        *count = token_ptr - tokens;
    }

    free(buffer);

    return tokens;
}

size_t rstrip(char *str, const char *chars)
{
    size_t str_len = strlen(str);

    if (!str_len)
    {
        return 0;
    }

    size_t chars_count = strlen(chars);

    size_t offset;
    size_t i;

    for (offset = str_len - 1; offset >= 0; offset--)
    {
        for (i = 0; i < chars_count; i++)
        {
            if (str[offset] == chars[i])
            {
                break;
            }
        }

        if (i == chars_count)
        {
            break;
        }
    }

    offset++;
    str[offset] = 0;

    return offset;
}

size_t strip(char *str, const char *chars)
{
    size_t chars_count = strlen(chars);
    size_t str_len = strlen(str);

    size_t offset;
    size_t i;

    for (offset = 0; offset < str_len; offset++)
    {
        for (i = 0; i < chars_count; i++)
        {
            if (str[offset] == chars[i])
            {
                break;
            }
        }

        if (i == chars_count)
        {
            break;
        }
    }

    if(offset)
    {   
        strcpy(str, str + offset);
    }   

    return rstrip(str, chars);
}