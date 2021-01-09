#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <linux/limits.h>

char *get_dirname(const char *basename, char *dirname)
{
    char *buffer;
    size_t len = strlen(basename);

    if(dirname == NULL)
    {
        buffer = (char *)malloc(len * sizeof(char));
    }
    else 
    {
        buffer = dirname;
    }

    int i;
    for (i = len; i >= 0; i--)
    {
        if (basename[i] == '/')
        {
            buffer[i] = 0;
            break;
        }
    }

    for (i--; i >= 0; i--)
    {
        buffer[i] = basename[i];
    }

    return buffer;
}

char *get_exe_path()
{
    char *path = (char *)malloc(PATH_MAX * sizeof(char));
    memset(path, 0, PATH_MAX);

    readlink("/proc/self/exe", path, PATH_MAX);

    return path;
}

char *get_exe_dir()
{
    char *exe_path = (char *)malloc(PATH_MAX * sizeof(char));
    memset(exe_path, 0, PATH_MAX);

    readlink("/proc/self/exe", exe_path, PATH_MAX);

    return get_dirname(exe_path, NULL);
}

char *get_home()
{
    struct passwd *pw = getpwuid(getuid());

    return pw->pw_dir;
}

char *home_relative(char *path, char *relative)
{
    char *home = get_home();
    char *ptr = strstr(path, home);

    char *buffer;

    if (relative != NULL)
    {
        buffer = relative;
    }
    else
    {
        buffer = (char *)malloc(strlen(path) * sizeof(char));
    }

    if (ptr && ptr == path)
    {
        strcpy(buffer + 1, path + strlen(home));
        *buffer = '~';
        
        return buffer;
    }
    else
    {
        strcpy(buffer, path);
        return buffer;
    }
}
