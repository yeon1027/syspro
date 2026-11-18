#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int pid;

    pid = fork();

    if (pid == 0) {
        printf("[Child] : Hello, world pid=%d\n", getpid());
    } else {
        printf("[Parent] : Hello, world pid=%d\n", getpid());
    }

    return 0;
}
