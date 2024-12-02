#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // wait 함수의 헤더 파일
#include <string.h>   // strcspn 함수의 헤더 파일

void execute_command(const char *command) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork 실패");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 자식 프로세스: 명령어 실행
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        perror("execl 실패");
        exit(EXIT_FAILURE);
    } else {
        // 부모 프로세스: 자식 프로세스가 종료될 때까지 대기
        wait(NULL);
    }
}

int main() {
    char command[256];

    printf("실행할 명령어를 입력하세요: ");
    if (fgets(command, sizeof(command), stdin) != NULL) {
        // 입력받은 명령어의 끝에 개행 문자가 포함되어 있을 경우 제거
        command[strcspn(command, "\n")] = 0;

        if (command[0] != '\0') {
            execute_command(command);
        } else {
            printf("명령어를 입력하지 않았습니다.\n");
        }
    } else {
        perror("명령어 입력 실패");
    }

    return 0;
}

