#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#define LOOPTIMES 1000000
#define ERREXIT(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

struct timeval STime, ETime;
struct timeval pipe1ReadTime, pipe1WriteTime, pipe2ReadTime, pipe2WriteTime;

char *text = "text test test \t";
char buf[40];

void printTimeInterval(struct timeval *STime, struct timeval *ETime)
{
    printf("%ld.%ld %ld.%ld \n", STime->tv_sec, STime->tv_usec, ETime->tv_sec, ETime->tv_usec);
    if (ETime->tv_sec == STime->tv_sec) {
        printf("time interval %ld ms\n", ETime->tv_usec - STime->tv_usec);
    } else {
        printf("time interval %ld ms\n", (ETime->tv_sec - STime->tv_sec ) * 1000 + ETime->tv_usec - STime->tv_usec);
    }
}

void timeToBuf(struct timeval* time, char *buf) 
{
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%ld.%ld", time->tv_sec, time->tv_usec);
    // printf("buf: %s\n", buf);
} 

void bufToTime(struct timeval* time, char *buf)
{
    char *first, *second;
    first = strtok(buf, ".");
    second = strtok(NULL, ".");
    // printf("first:%s, second:%s\n", first, second);
    time->tv_sec = atol(first);
    time->tv_usec = atol(second);
}

void cost_system_call()
{
    gettimeofday(&STime, NULL);
    write(STDOUT_FILENO, text, strlen(text));
    gettimeofday(&ETime, NULL);
    printTimeInterval(&STime, &ETime);
}

void cost_context_switch()
{
    int pipefd1[2], pipefd2[2];
    int res, waitStatus, i;
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(1, &set);

    if ( pipe(pipefd1) < 0)
        ERREXIT("pipe1 failed");
    if (pipe(pipefd2) < 0)
        ERREXIT("pipe2 failed");

    res = fork();
    if (res < 0) {
        ERREXIT("fork failed");
    } else if (res == 0) {
        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
            ERREXIT("set child1 process to cpu1 failed");
        close(pipefd1[0]); // close pipe1 read; pipe2 write
        close(pipefd2[1]);
        for (i = 0; i < LOOPTIMES; i++) {
            gettimeofday(&pipe1WriteTime, NULL);
            timeToBuf(&pipe1WriteTime, buf);
            write(pipefd1[1], buf, strlen(buf)); // write pipe1
            printf("[child1] write to pipe1, %ld.%ld\n", pipe1WriteTime.tv_sec, pipe1WriteTime.tv_usec);
            // printf("[child1] wait read pipe2, %ld.%ld\n", pipe1WriteTime.tv_sec, pipe1WriteTime.tv_usec);
            memset(buf, 0, sizeof(buf));
            read(pipefd2[0], buf, sizeof(buf)); // wait read pipe2
            gettimeofday(&pipe2ReadTime, NULL);
            bufToTime(&pipe2WriteTime, buf);
            printf("[child1] read from pipe2, %ld.%ld\n", pipe2ReadTime.tv_sec, pipe2ReadTime.tv_usec);
            printTimeInterval(&pipe2WriteTime, &pipe2ReadTime);
        }
        
    } else {
        res = fork();
        if (res < 0) {
            ERREXIT("fork failed");
        } else if (res == 0) {
            if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
                ERREXIT("set child2 process to cpu1 failed");
            close(pipefd1[1]); // colse pipe1 write; pipe read;
            close(pipefd2[0]);
            for (i = 0; i < LOOPTIMES; i++) {
                gettimeofday(&pipe2WriteTime, NULL);
                timeToBuf(&pipe2WriteTime, buf);
                write(pipefd2[1], buf, sizeof(buf)); // write pipe2
                printf("[child2] write to pipe2, %ld.%ld\n", pipe2WriteTime.tv_sec, pipe2WriteTime.tv_usec);
                // printf("[child2] wait read pipe1, %ld.%ld\n", pipe2WriteTime.tv_sec, pipe2WriteTime.tv_usec);
                memset(buf, 0 , sizeof(buf));
                read(pipefd1[0], buf, sizeof(buf)); // wait read pipe1
                gettimeofday(&pipe1ReadTime, NULL);
                bufToTime(&pipe1WriteTime, buf);
                printf("[child2] read from pipe1, %ld.%ld\n", pipe1ReadTime.tv_sec, pipe1ReadTime.tv_usec);
                printTimeInterval(&pipe1WriteTime, &pipe1ReadTime);
            }
        }
    }
    
    while ( ( res = wait(&waitStatus) ) > 0) {
        printf("[parent] wait res = %d\n", res);
    }
}

int main()
{
    int i = 0;
    // test system call cost time
    // for (; i < 100 ; i++) {
    //     cost_system_call();
    // }
    cost_context_switch();

    // test timeToBuf(), bufToTime()
    // gettimeofday(&STime, NULL);
    // timeToBuf(&STime, buf);
    // bufToTime(&STime, buf);


    return 0;
}