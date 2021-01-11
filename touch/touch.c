#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int touch(const char *filename)
{
    int file_fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

    if(file_fd < 0)
    {
        dprintf(2, "Failed to open file %s.\n", filename);
        return -1;
    }

    close(file_fd);

    return 0;
}

int main(int argc, char **argv)
{

    if(argc < 2)
    {
        dprintf(2, "You have to provide a path.\n");

        return -1;
    }

    return touch(argv[1]);
}