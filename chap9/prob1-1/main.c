#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int pid;

    printf("[%d] Process start\n", getpid());

    pid = fork();

    printf("[%d] Process : return value %d\n", getpid(), pid);

    return 0;
}
