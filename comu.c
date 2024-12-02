#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        close(fd[0]);
        write(fd[1], "Hello from child", 16);
        close(fd[1]);
    } else {
        char buffer[20];
        close(fd[1]);
        read(fd[0], buffer, 20);
        printf("부모 프로세스에서 읽음: %s\n", buffer);
        close(fd[0]);
    }

    return 0;
}

