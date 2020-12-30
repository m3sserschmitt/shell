#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <linux/limits.h>

void get_dirname(const char *basename, char *dirname)
{
    int i;
    for (i = strlen(basename); i >= 0; i--)
    {
        if (basename[i] == '/')
        {
            dirname[i] = 0;
            break;
        }
    }

    for (i--; i >= 0; i--)
    {
        dirname[i] = basename[i];
    }
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

    char *dirname = (char *)malloc(strlen(exe_path) * sizeof(char));

    get_dirname(exe_path, dirname);

    return dirname;
}

char *get_home()
{
    struct passwd *pw = getpwuid(getuid());

    return pw->pw_dir;
}

void home_relative(char *path, char *relative)
{
    char *home = get_home();
    char *ptr = strstr(path, home);

    if(ptr && ptr == path)
    {
        char *buffer = (char *)malloc(strlen(path) * sizeof(char));
        strcpy(buffer, path + strlen(home));
        
        *relative = '~';
        strcpy(relative + 1, buffer);
        

        free(buffer);
    }
}
