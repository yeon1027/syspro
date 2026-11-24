#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void execute(char *cmd)
{
    char *args[128];
    char *infile = NULL;
    char *outfile = NULL;
    char *save;
    char *word;
    pid_t pid;
    int argc = 0;
    int background = 0;
    int fd;

    word = strtok_r(cmd, " \t\r\n", &save);

    while (word != NULL) {
        if (strcmp(word, "<") == 0 || strcmp(word, ">") == 0) {
            char direction = word[0];

            word = strtok_r(NULL, " \t\r\n", &save);

            if (word == NULL || strcmp(word, "<") == 0 ||
                strcmp(word, ">") == 0 || strcmp(word, "&") == 0) {
                fprintf(stderr, "Missing file name\n");
                return;
            }

            if (direction == '<')
                infile = word;
            else
                outfile = word;
        } else if (strcmp(word, "&") == 0) {
            background = 1;

            if (strtok_r(NULL, " \t\r\n", &save) != NULL) {
                fprintf(stderr, "& must be at the end of the command\n");
                return;
            }

            break;
        } else {
            if (argc == 127) {
                fprintf(stderr, "Too many arguments\n");
                return;
            }

            args[argc++] = word;
        }

        word = strtok_r(NULL, " \t\r\n", &save);
    }

    if (argc == 0)
        return;

    args[argc] = NULL;
    pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        if (infile != NULL) {
            fd = open(infile, O_RDONLY);
            if (fd < 0 || dup2(fd, STDIN_FILENO) < 0) {
                perror(infile);
                _exit(1);
            }

            if (fd != STDIN_FILENO)
                close(fd);
        }

        if (outfile != NULL) {
            fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd < 0 || dup2(fd, STDOUT_FILENO) < 0) {
                perror(outfile);
                _exit(1);
            }

            if (fd != STDOUT_FILENO)
                close(fd);
        }

        execvp(args[0], args);
        perror(args[0]);
        _exit(1);
    }

    if (!background)
        waitpid(pid, NULL, 0);
}

int main(void)
{
    char line[1024];
    char *cmd;
    char *save;

    while (1) {
        while (waitpid(-1, NULL, WNOHANG) > 0)
            ;

        printf("[shell] ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        cmd = strtok_r(line, ";", &save);

        while (cmd != NULL) {
            execute(cmd);
            cmd = strtok_r(NULL, ";", &save);
        }
    }

    return 0;
}
