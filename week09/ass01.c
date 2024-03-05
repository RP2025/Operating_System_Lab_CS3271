#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void myfind(char *directory);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    myfind(argv[1]);
    return 0;
}

void myfind(char *directory) {
    DIR *dir;
    struct dirent *entry;
    char full_path[PATH_MAX];

    if ((dir = opendir(directory)) == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);
        printf("%s\n", full_path);

        if (entry->d_type == DT_DIR) {
            myfind(full_path);
        }
    }

    closedir(dir);
}


