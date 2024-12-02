#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long type;
    char text[100];
};

int main() {
    key_t key = ftok("chat", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    struct message msg;

    while (1) {
        msgrcv(msgid, &msg, sizeof(msg), 1, 0);
        printf("클라이언트: %s\n", msg.text);

        printf("서버: ");
        fgets(msg.text, sizeof(msg.text), stdin);
        msg.type = 2;
        msgsnd(msgid, &msg, sizeof(msg), 0);
    }

    return 0;
}

