#include <sys/types.h>

size_t str_count(const char *str_in, const char *str);

char **split(const char *str, const char *delim, int *count);

size_t rstrip(char *str, const char *chars);

size_t strip(char *str, const char *chars);