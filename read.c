#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char filename[256] = "example.txt";
    char buffer[256];

    // 파일 쓰기
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("파일 열기 실패");
        return 1;
    }
    fprintf(file, "Hello, 파일 입출력 연습!\n");
    fclose(file);
    printf("파일에 내용이 저장되었습니다: %s\n", filename);

    // 파일 읽기
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("파일 읽기 실패");
        return 1;
    }
    printf("파일 내용:\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);

    return 0;
}

