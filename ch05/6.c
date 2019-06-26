#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{

    int res = fork();
    if (res < 0) {
        perror("fork failed!\n");
        return -1;
    } else if (res == 0) {
        printf("[child] pid = %d\n", getpid());
        // int *status = NULL;
        // pid_t  res = wait(status);
        // printf("[child] wait() res = %d\n", res);
    } else {
        int *status = NULL;
        pid_t  res = waitpid(-1, status, 0);
        printf("[parent] waitpid() = %d \n", res);
    }
    return 0;
}