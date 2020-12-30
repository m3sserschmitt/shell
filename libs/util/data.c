#include <unistd.h>
#include <stdlib.h>
#include <string.h>

ssize_t read_data(int fd, char **recv_data, int stdoutw)
{
    const size_t block_size = 1024;
    size_t current_size = block_size;

    *recv_data = (char *)malloc((current_size + 1) * sizeof(char));
    memset(*recv_data, 0, current_size + 1);

    char *recv_buffer = (char *)malloc(block_size * sizeof(char));
    memset(recv_buffer, 0, block_size);

    ssize_t total_received = 0;
    size_t received = 0;

    while ((received = read(fd, recv_buffer, block_size)) > 0)
    {
        if (stdoutw)
        {
            write(1, recv_buffer, strlen(recv_buffer));
        }

        if (total_received + received > current_size)
        {
            current_size += block_size;
            *recv_data = realloc(*recv_data, (current_size + 1) * sizeof(char));

            memset(*recv_data + total_received, 0, current_size - total_received + 1);
        }

        total_received += received;

        strcat(*recv_data, recv_buffer);
        memset(recv_buffer, 0, block_size);
    }

    free(recv_buffer);

    return received < 0 ? -1 : total_received;
}