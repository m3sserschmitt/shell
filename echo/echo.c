#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_MESSAGE_LEN 4096

int echo(int argc, char **argv)
{
    char *message = (char *)malloc(MAX_MESSAGE_LEN * sizeof(char));
    memset(message, 0, MAX_MESSAGE_LEN);
    int file_fd = 0;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp("-", *(argv + i)) == 0)
        {
            file_fd = open(*(argv + i + 1), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        }
        else if (strcmp("--", *(argv + i)) == 0)
        {
            file_fd = open(*(argv + i + 1), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        }

        if (file_fd)
        {
            if (file_fd < 0)
            {
                dprintf(2, "Failed to open file %s.\n", *(argv + i + 1));

                return -1;
            }

            strcat(message, "\n");

            int result = write(file_fd, message, strlen(message)) > 0;

            close(file_fd);
            free(message);

            return result ? 0 : -1;
        }

        strcat(message, *(argv + i));

        if (i < argc - 1 && strcmp("-", *(argv + i + 1)) != 0 && strcmp("--", *(argv + i)) != 0)
        {
            strcat(message, " ");
        }
    }

    printf("%s\n", message);
    free(message);

    return 0;
}

int main(int argc, char **argv)
{

    return echo(argc - 1, argv + 1);
}