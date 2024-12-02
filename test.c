#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void list_dir(const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    printf("\n%s:\n", path);

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        printf("%s  ", entry->d_name);

        // 디렉토리일 경우 재귀 호출
        if (entry->d_type == DT_DIR) {
            char new_path[1024];
            snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);
            list_dir(new_path);
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {
    const char *start_path = argc > 1 ? argv[1] : ".";
    list_dir(start_path);
    return 0;
}

