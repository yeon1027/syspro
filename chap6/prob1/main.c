#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    struct stat buf;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s file...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (lstat(argv[i], &buf) == -1) {
            perror("lstat()");
            continue;
        }

        printf("%s: ", argv[i]);

        if (S_ISREG(buf.st_mode))
            printf("Regular File\n");
        else if (S_ISDIR(buf.st_mode))
            printf("Directory\n");
        else if (S_ISCHR(buf.st_mode))
            printf("Character Device\n");
        else if (S_ISBLK(buf.st_mode))
            printf("Block Device\n");
        else if (S_ISFIFO(buf.st_mode))
            printf("FIFO\n");
        else if (S_ISLNK(buf.st_mode))
            printf("Symbolic Link\n");
        else if (S_ISSOCK(buf.st_mode))
            printf("Socket\n");
        else
            printf("Unknown\n");
    }

    return 0;
}

