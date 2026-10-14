#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s mode file\n", argv[0]);
        return 1;
    }

    char *end;
    long mode = strtol(argv[1], &end, 8);

    if (argv[1][0] == '\0' || *end != '\0' || mode < 0 || mode > 07777) {
        fprintf(stderr, "Invalid mode\n");
        return 1;
    }

    if (chmod(argv[2], (mode_t)mode) == -1) {
        perror(argv[2]);
        return 1;
    }

    return 0;
}

