#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

int _kill(int argc, char **argv)
{
    // kill pid signal (signal = 9, kill process)

    // suspend signal = 17
    // resume signal = 19
    

    if(argc < 1)
    {
        dprintf(2, "Error: You have to provid a PID.\n");

        return -1;
    }

    int signal = 9;

    if (argc == 2)
    {
        signal = atoi(argv[1]);
    }

    return kill(atoi(argv[0]), signal);
}

int main(int argc, char **argv)
{

    return _kill(argc - 1, argv + 1);
}