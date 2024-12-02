#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void typing_practice() {
    char sentences[][100] = {
        "The quick brown fox jumps over the lazy dog.",
        "Hello, this is a typing test.",
        "Practice makes perfect."
    };
    int num_sentences = 3;

    int errors = 0;
    double total_time = 0;
    char input[100];

    for (int i = 0; i < num_sentences; i++) {
        printf("\n문장: %s\n", sentences[i]);
        printf("타이핑 시작: ");

        clock_t start_time = clock();
        fgets(input, sizeof(input), stdin);
        clock_t end_time = clock();

        // 시간 측정
        total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;

        // 오타 측정
        for (int j = 0; j < strlen(sentences[i]); j++) {
            if (input[j] != sentences[i][j]) {
                errors++;
            }
        }
    }

    double wpm = (60.0 / total_time) * num_sentences;

    printf("\n결과: \n");
    printf("오타 수: %d\n", errors);
    printf("평균 분당 타자수(WPM): %.2f\n", wpm);
}

int main() {
    typing_practice();
    return 0;
}

