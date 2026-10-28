#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "student.h"

int main(int argc, char *argv[])
{
    int fd, id;
    struct student record;
    struct flock lock = {0};

    if (argc < 2) {
        fprintf(stderr, "How to use : %s File\n", argv[0]);
        return 1;
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        return 2;
    }

    printf("\nEnter StudentID to search : ");
    while (scanf("%d", &id) == 1) {
        if (id < START_ID) {
            printf("No record %d\n", id);
            printf("\nEnter StudentID to search : ");
            continue;
        }

        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = ((off_t)id - START_ID) * sizeof(record);
        lock.l_len = sizeof(record);

        if (fcntl(fd, F_SETLKW, &lock) == -1) {
            perror(argv[1]);
            close(fd);
            return 3;
        }

        if (lseek(fd, lock.l_start, SEEK_SET) == -1) {
            perror(argv[1]);
            close(fd);
            return 4;
        }

        ssize_t n = read(fd, &record, sizeof(record));

        if (n == -1) {
            perror(argv[1]);
            close(fd);
            return 5;
        }

        if (n == sizeof(record) && record.id == id) {
            printf("Name:%s\t StuID:%d\t Score:%d\n",
                   record.name, record.id, record.score);
        } else {
            printf("No record %d\n", id);
        }

        lock.l_type = F_UNLCK;

        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror(argv[1]);
            close(fd);
            return 6;
        }

        printf("\nEnter StudentID to search : ");
    }

    close(fd);
    return 0;
}
