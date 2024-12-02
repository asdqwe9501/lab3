#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *filename = "temp_file.txt";

    // 파일 생성
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("파일 생성 실패");
        return 1;
    }
    fprintf(file, "임시 파일입니다.\n");
    fclose(file);
    printf("파일이 생성되었습니다: %s\n", filename);

    // 파일 삭제
    if (remove(filename) == 0) {
        printf("파일이 삭제되었습니다: %s\n", filename);
    } else {
        perror("파일 삭제 실패");
    }

    return 0;
}

