#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main ()
{
    close(STDOUT_FILENO); // close standard output
    int fd = open("./testFile", O_CREAT| O_WRONLY|O_TRUNC, 0777);
    int res = fork();
    
    if (res < 0) {
        perror("fork failed\n");
        return -1;
    } else if (res == 0) { // child process
        // printf("standard output fd = %d \n", STDOUT_FILENO);
        
        printf("in child process, fd = %d\n" ,fd);
        // char *args[3];
        // args[0] = strdup("ls");
        // args[1] = NULL;
        // execvp(args[0], args);
    } else { // parent process
        wait(NULL);
    }
    // printf("After close standard output ,open file fd = %d \n", fd);
}