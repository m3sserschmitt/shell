#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char *get_hostname()
{
    char *hostname = (char *)malloc(256 * sizeof(char));

    gethostname(hostname, 256);

    return hostname;
}

char *get_user()
{
    char *buffer = getlogin();

    char *user = (char *)malloc((strlen(buffer) + 1) * sizeof(char));

    strcpy(user, buffer);

    return user;
}
