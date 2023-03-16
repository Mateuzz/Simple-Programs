#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#undef bool
typedef _Bool bool;

bool *getFilterByStringTerminator(
    char **list, size_t listSize, const char *sufix) {
    bool *filter = (bool *)calloc(listSize, sizeof(bool));
    if (!filter) {
        return NULL;
    }

    size_t suffixLenght = strlen(sufix);

    for (size_t i = 0; i < listSize; ++i) {
        char *currentSuffix = &list[i][strlen(list[i]) - suffixLenght];
        if (currentSuffix >= &list[i][0] && !strcmp(currentSuffix, sufix)) {
            filter[i] = true;
        }
    }

    return filter;
}

int getFilenamesCurrentDirectory(char ***filenames) {
    FILE *fileListDirectory = popen("ls -R", "r");
    if (!fileListDirectory) {
        return -1;
    }

    size_t capacity = 10;
    int    size     = 0;
    char   nameBuff[200];
    char   directoryname[202] = "./";

    *filenames = malloc(sizeof(char *) * capacity);
    if (!*filenames) {
        return -1;
    }

    while (fgets(nameBuff, 200, fileListDirectory)) {
        strtok(nameBuff, "\n");
        if (strcspn(nameBuff, ":") == strlen(nameBuff) - 1) {
            strtok(nameBuff, ":");
            sprintf(directoryname, "%s/", nameBuff);
        } else {
            if (size == capacity) {
                capacity += capacity;
                *filenames = realloc(*filenames, sizeof(char *) * capacity);
                if (!*filenames) {
                    return -1;
                }
            }
            (*filenames)[size] =
                malloc(strlen(nameBuff) + strlen(directoryname) + 1);
            if (!(*filenames)[size]) {
                return size;
            }
            strcpy((*filenames)[size], directoryname);
            strcat((*filenames)[size], nameBuff);
            ++size;
        }
    }

    *filenames = realloc(*filenames, sizeof(char *) * size);

    return size;
}

int countLinesFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    assert(fp != NULL);

    char buff[1000];
    int  lines = 0;

    while (fgets(buff, 1000, fp)) {
        ++lines;
    }

    fclose(fp);

    return lines;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: [program] [extension](s)\n");
        return 1;
    }

    const char *filter        = argv[1];
    char **     filenames     = NULL;
    bool *      filteredFiles = NULL;
    int         filesCount    = 0;
    int         totalLines    = 0;

    if ((filesCount = getFilenamesCurrentDirectory(&filenames)) <= 0) {
        goto Exit;
    }

    if (!(filteredFiles = getFilterByStringTerminator(filenames, filesCount, filter))) {
        goto Exit;
    }

    for (int i = 0; i < filesCount; ++i) {
        if (filteredFiles[i]) {
            int lines = countLinesFile(filenames[i]);
            printf("File: %s   -  lines: %d\n", filenames[i], lines);
            totalLines += lines;
        }
    }

    printf("\nTotal lines: %d\n", totalLines);

Exit:
    for (int i = 0; i < filesCount; ++i) {
        free(filenames[i]);
    }

    free(filenames);
    free(filteredFiles);
}
