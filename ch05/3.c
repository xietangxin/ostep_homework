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
        printf("[child]  hello\n");
    } else {
        sleep(1);
        // wait(NULL);
        printf("[parent]  goodbye\n");
    }
    return 0;
}