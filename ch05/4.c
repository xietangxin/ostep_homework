// #define _GNU_SOURCE

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
        printf("[child] \n");
        // execl("/bin/ls", "ls", "-l", (char *)NULL); // execl 必须将参数一个个列出
        // execle("/bin/ls", "ls", "-l", (char *)NULL, NULL); // 
        // execlp("/bin/ls", "ls", "-l", (char *)NULL);
        char *args[3];
        args[0] = "ls";
        args[1] = "-l";
        args[2] = NULL;
        // execv("/bin/ls", args);
        // execvp("/bin/ls", args);
        execvpe("/bin/ls", args, (char *)NULL);
    } else {
        wait(NULL);
        printf("[parent] \n");
    }
    return 0;
}