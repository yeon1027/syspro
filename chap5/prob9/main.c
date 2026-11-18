#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LINES 100
#define MAX_LENGTH 1024

int main(int argc, char *argv[])
{
    int fd;
    char savedText[MAX_LINES][MAX_LENGTH] = {0};
    char buf;
    ssize_t nread;
    int total = 0;
    int column = 0;
    int i;

    if (argc != 2) {
        fprintf(stderr, "사용법: %s 파일명\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        perror(argv[1]);
        return 1;
    }

    while ((nread = read(fd, &buf, 1)) > 0) {
        if (total >= MAX_LINES) {
            fprintf(stderr, "저장 가능한 줄 수를 초과했습니다.\n");
            close(fd);
            return 1;
        }

        if (buf == '\n') {
            savedText[total][column] = '\0';
            total++;
            column = 0;
        } else {
            if (column >= MAX_LENGTH - 1) {
                fprintf(stderr, "한 줄의 길이가 너무 깁니다.\n");
                close(fd);
                return 1;
            }

            savedText[total][column++] = buf;
        }
    }

    if (nread == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    if (column > 0) {
        savedText[total][column] = '\0';
        total++;
    }

    close(fd);

    for (i = total - 1; i >= 0; i--) {
        printf("%s\n", savedText[i]);
    }

    return 0;
}

