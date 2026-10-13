#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LINES 100
#define MAX_LENGTH 1024

int get_line_number(const char *text, int total)
{
    char *end;
    long number;

    while (isspace((unsigned char)*text)) {
        text++;
    }

    if (*text == '\0') {
        return -1;
    }

    number = strtol(text, &end, 10);

    if (text == end) {
        return -1;
    }

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (*end != '\0' || number < 1 || number > total) {
        return -1;
    }

    return (int)number;
}

int main(int argc, char *argv[])
{
    int fd;
    char savedText[MAX_LINES][MAX_LENGTH] = {0};
    char buf;
    ssize_t nread;

    int total = 0;
    int column = 0;

    char input[256];
    char *dash;
    char *token;

    int selected[MAX_LINES];
    int count = 0;
    int number;
    int start;
    int finish;
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

            savedText[total][column] = buf;
            column++;
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

    printf("File read success\n");
    printf("Total Line : %d\n", total);

    if (total == 0) {
        printf("파일이 비어 있습니다.\n");
        return 0;
    }

    printf("You can choose 1 ~ %d Line\n", total);
    printf("Pls 'Enter' the line to select : ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "입력 오류\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "*") == 0) {
        for (i = 0; i < total; i++) {
            selected[count++] = i + 1;
        }
    } else if (strchr(input, ',') != NULL) {
        size_t length = strlen(input);

        if (input[0] == ',' ||
            input[length - 1] == ',' ||
            strstr(input, ",,") != NULL) {
            fprintf(stderr, "입력 오류\n");
            return 1;
        }

        token = strtok(input, ",");

        while (token != NULL) {
            number = get_line_number(token, total);

            if (number == -1 || count >= MAX_LINES) {
                fprintf(stderr, "입력 오류\n");
                return 1;
            }

            selected[count++] = number;
            token = strtok(NULL, ",");
        }
    } else if ((dash = strchr(input, '-')) != NULL) {
        *dash = '\0';

        start = get_line_number(input, total);
        finish = get_line_number(dash + 1, total);

        if (start == -1 || finish == -1 || start > finish) {
            fprintf(stderr, "입력 오류\n");
            return 1;
        }

        for (i = start; i <= finish; i++) {
            selected[count++] = i;
        }
    } else {
        number = get_line_number(input, total);

        if (number == -1) {
            fprintf(stderr, "입력 오류\n");
            return 1;
        }

        selected[count++] = number;
    }

    for (i = 0; i < count; i++) {
        number = selected[i];
        printf("%d %s\n", number, savedText[number - 1]);
    }

    return 0;
}

