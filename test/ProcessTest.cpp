

#include "gtest/gtest.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int _19_04_05_test(){

    pid_t processId;
    if ((processId = fork()) == 0) { // 父进程返回子进程 pid，子进程返回0
        printf("child_fork_return_value :%d\n", processId);
        printf("child_pid: %d\n", getpid());
        char app[]         = "/bin/echo";
        char *const argv[] = { app, "success", NULL };
        if (execv(app, argv) < 0) {
            perror("execv error");
        }
    } else if (processId < 0) {
        perror("fork error");
    } else {
        printf("parent_fork_return_value :%d\n", processId);
        printf("parent_pid: %d\n", getpid());
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

TEST(ProcessTest,test_use){
    _19_04_05_test();
}
