#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARG 64

int main(void)
{
    char line[1024];
    char *args[MAXARG];
    char *word;
    char *save;
    pid_t pid;
    pid_t ended;
    int argc;
    int background;
    int status;
    size_t length;

    while (1) {
        while ((ended = waitpid(-1, &status, WNOHANG)) > 0) {
            printf("[%ld] child process end %ld\n",
                   (long)getpid(), (long)ended);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                puts("SUCCESS");
        }

        printf("Pls input cmd : ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        length = strlen(line);

        while (length > 0 && strchr(" \t\r\n", line[length - 1]) != NULL)
            line[--length] = '\0';

        background = 0;

        if (length > 0 && line[length - 1] == '&') {
            line[length - 1] = '\0';
            background = 1;
        }

        argc = 0;
        word = strtok_r(line, " \t", &save);

        while (word != NULL && argc < MAXARG - 1) {
            args[argc++] = word;
            word = strtok_r(NULL, " \t", &save);
        }

        if (word != NULL) {
            fprintf(stderr, "Too many arguments\n");
            continue;
        }

        if (argc == 0)
            continue;

        args[argc] = NULL;

        printf("[%ld] Parent process start\n", (long)getpid());
        fflush(stdout);

        pid = fork();

        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            printf("[%ld] child process start\n", (long)getpid());
            fflush(stdout);

            execvp(args[0], args);
            perror(args[0]);
            _exit(127);
        } else if (!background) {
            if (waitpid(pid, &status, 0) < 0) {
                perror("waitpid");
            } else {
                printf("[%ld] child process end %ld\n",
                       (long)getpid(), (long)pid);

                if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                    puts("SUCCESS");
            }
        }
    }

    puts("\nExit");
    return 0;
}
