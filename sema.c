#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

#define SHM_NAME "/file_copy_shm"
#define SEM_PARENT "/sem_parent"
#define SEM_CHILD "/sem_child"
#define BUFFER_SIZE 1024

void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *source_file = argv[1];
    const char *destination_file = argv[2];

    // 공유 메모리와 세마포어 초기화
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) handle_error("shm_open");

    if (ftruncate(shm_fd, BUFFER_SIZE) == -1) handle_error("ftruncate");

    char *shared_mem = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) handle_error("mmap");

    sem_t *sem_parent = sem_open(SEM_PARENT, O_CREAT, 0666, 1);
    sem_t *sem_child = sem_open(SEM_CHILD, O_CREAT, 0666, 0);

    if (sem_parent == SEM_FAILED || sem_child == SEM_FAILED) handle_error("sem_open");

    pid_t pid = fork();

    if (pid == -1) {
        handle_error("fork");
    } else if (pid == 0) {
        // 자식 프로세스 (쓰기)
        FILE *dest = fopen(destination_file, "w");
        if (!dest) handle_error("fopen (destination)");

        while (1) {
            sem_wait(sem_child); // 부모가 데이터 준비를 완료했는지 대기

            if (strncmp(shared_mem, "EOF", 3) == 0) break;

            fwrite(shared_mem, 1, strlen(shared_mem), dest);
            sem_post(sem_parent); // 부모에게 신호
        }

        fclose(dest);
        printf("자식: 파일 복사가 완료되었습니다.\n");

        // 자원 정리
        munmap(shared_mem, BUFFER_SIZE);
        shm_unlink(SHM_NAME);
        sem_close(sem_parent);
        sem_close(sem_child);
        sem_unlink(SEM_PARENT);
        sem_unlink(SEM_CHILD);

        exit(EXIT_SUCCESS);
    } else {
        // 부모 프로세스 (읽기)
        FILE *src = fopen(source_file, "r");
        if (!src) handle_error("fopen (source)");

        while (1) {
            sem_wait(sem_parent); // 자식이 데이터를 읽기를 기다림

            size_t bytes_read = fread(shared_mem, 1, BUFFER_SIZE - 1, src);
            shared_mem[bytes_read] = '\0';

            if (bytes_read == 0) { // EOF
                strncpy(shared_mem, "EOF", 3);
                sem_post(sem_child);
                break;
            }

            sem_post(sem_child); // 자식에게 데이터 준비 완료 신호
        }

        fclose(src);
        wait(NULL); // 자식 프로세스 종료 대기
        printf("부모: 파일 복사가 완료되었습니다.\n");

        // 자원 정리
        munmap(shared_mem, BUFFER_SIZE);
        shm_unlink(SHM_NAME);
        sem_close(sem_parent);
        sem_close(sem_child);
        sem_unlink(SEM_PARENT);
        sem_unlink(SEM_CHILD);

        exit(EXIT_SUCCESS);
    }

    return 0;
}

