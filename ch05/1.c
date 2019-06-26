#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int x = 100;

    int res = fork();
    if (res < 0) {
        perror("fork failed!\n");
        return -1;
    } else if (res == 0) {
        x = 20;
        printf("[child] x = %d\n", x);
    } else {
        x = 30;
        // wait(NULL);
        printf("[parent] x = %d \n", x);
    }
    return 0;
}