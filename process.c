#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("자식 프로세스: PID = %d\n", getpid());
        execlp("ls", "ls", "-l", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        printf("부모 프로세스: PID = %d\n", getpid());
        wait(NULL);
        printf("자식 프로세스 종료\n");
    } else {
        perror("fork");
    }

    return 0;
}

