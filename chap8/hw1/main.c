#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char *argv[])
{
    int i;
    char *value;

    if (argc < 2) {
        printf("사용법: %s -e [환경변수이름] | -u | -g | -i | -p\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0) {
        if (argc == 2) {
            for (i = 0; environ[i] != NULL; i++) {
                printf("%s\n", environ[i]);
            }
        } else {
            value = getenv(argv[2]);

            if (value != NULL) {
                printf("%s = %s\n", argv[2], value);
            } else {
                printf("해당 환경변수가 없습니다.\n");
            }
        }
    } else if (strcmp(argv[1], "-u") == 0) {
        printf("Real User ID: %lu\n", (unsigned long)getuid());
        printf("Effective User ID: %lu\n", (unsigned long)geteuid());
    } else if (strcmp(argv[1], "-g") == 0) {
        printf("Real Group ID: %lu\n", (unsigned long)getgid());
        printf("Effective Group ID: %lu\n", (unsigned long)getegid());
    } else if (strcmp(argv[1], "-i") == 0) {
        printf("Process ID: %ld\n", (long)getpid());
    } else if (strcmp(argv[1], "-p") == 0) {
        printf("Parent Process ID: %ld\n", (long)getppid());
    } else {
        printf("잘못된 옵션입니다.\n");
        return 1;
    }

    return 0;
}
