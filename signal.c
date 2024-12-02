#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig) {
    printf("시그널 %d을 받았습니다.\n", sig);
}

int main() {
    signal(SIGINT, signal_handler);

    printf("Ctrl+C를 눌러 시그널을 테스트하세요.\n");
    while (1) {
        pause();
    }
    return 0;
}

