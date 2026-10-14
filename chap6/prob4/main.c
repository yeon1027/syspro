#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    DIR *dp;
    char *dir;
    struct dirent *d;

    if (argc == 1)
        dir = ".";
    else
        dir = argv[1];

    if ((dp = opendir(dir)) == NULL) {
        perror(dir);
        exit(1);
    }

    while ((d = readdir(dp)) != NULL)
        printf("%s %lu\n", d->d_name, (unsigned long)d->d_ino);

    closedir(dp);
    exit(0);
}

