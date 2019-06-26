#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main()
{   
    int pipefd[2];

    if (pipe(pipefd)  == -1) {
        perror("pipe() failed\n");
        return -1;
    }
    printf("[parent] pipefd[0] = %d  pipefd[1] = %d \n", pipefd[0], pipefd[1]);

    int res = fork();
    if (res < 0) {
        perror("fork failed!\n");
        return -1;
    } else if (res == 0) {
        printf("[child0] pid = %d\n", getpid());
        // close(STDOUT_FILENO);
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        // pipefd[1] = STDOUT_FILENO;
        // char *buf = "child0 write to pipefd[1]";
        //  write(pipefd[1], buf, strlen(buf));
        printf("child0 write to pipefd[1]");

    } else {
        res = fork();
        if (res < 0) {
            perror("fork failed\n");
        } else if (res == 0) {
            printf("[child1] pid = %d\n", getpid());
            close(pipefd[1]);
            // close(STDIN_FILENO);
            // pipefd[0] = STDIN_FILENO;
            dup2(pipefd[0], STDIN_FILENO);
            char buf[40];
            read(pipefd[0], buf, 40);
            printf("[child1] read from pipefd[0], buf: %s\n", buf);
        } 
    }
    int *status = NULL;
    while ( (res = wait(status)) > 0) {
        printf("[parent] wait() = %d \n", res);
    } 
    return 0;
}