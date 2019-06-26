#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LOOPTIMES 3

int main()
{
    printf("[parent] pid = %d\n", getpid());
    int i = 0, res;
    for (; i < LOOPTIMES; i++) {
        res = fork();
        if (res < 0) {
            perror("fork failed");
        } else if (res == 0) {
            printf("[child%d] pid = %d\n", i, getpid());
            // int res_wait = wait(NULL);
            // printf("[child%d] wait res = %d\n", i, res_wait);
        } else  {
            wait(NULL);
        }
    }
    return 0;
}