#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int _mkdir(char *path)
{
    struct stat st = {0};

    if (stat(path, &st) == -1)
    {
        return mkdir(path, 0777);
    }

    return -1;
}

int main(int argc, char **argv)
{

    if(argc < 2)
    {
        dprintf(2, "You have to provide a path.\n");

        return -1;
    }

    return _mkdir(argv[1]);
}
