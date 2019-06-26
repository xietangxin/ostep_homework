#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = open("./testFile", O_CREAT| O_WRONLY|O_TRUNC, 0777);

    int res = fork();
    if (res < 0) {
        perror("fork failed!\n");
        return -1;
    } else if (res == 0) {
        printf("[child] fd = %d\n", fd);
        char *msg = "child process\n";
        write(fd, msg, strlen(msg));
    } else {
        // wait(NULL);
        char *msg = "parent process\n";
        write(fd, msg, strlen(msg));
        printf("[parent] fd = %d \n", fd);
    }
    return 0;
}