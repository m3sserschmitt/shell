#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int cat(char *filename)
{

    int readFile_ID = open(filename, O_RDONLY);
    if (readFile_ID == -1)
    {

        dprintf(2, "Error: Cannot open file %s.\n", filename);
        return -1;
    }

    char *buffer = (char *)malloc(1024);

    while(read(readFile_ID, buffer, 1024) > 0)
    {
        printf("%s", buffer);
    }

    free(buffer);

    return 0;
}

int main(int argc, char **argv)
{

    if(argc < 2)
    {
        dprintf(2, "You have to provide a path.\n");

        return -1;
    }

    return cat(argv[1]);
}