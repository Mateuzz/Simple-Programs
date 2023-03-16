#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define PROCESSES_MAX 30
#define RESOURCES_MAX 10

typedef struct {
    int allocated[PROCESSES_MAX][RESOURCES_MAX];
    int max[PROCESSES_MAX][RESOURCES_MAX];
    int available[RESOURCES_MAX];
    int pcount;
    int rcount;
} BankersInfo;

void printBankers(const BankersInfo *info) {
    printf("{ ");
    for (int i = 0; i < info->rcount; ++i) {
        printf("%d ", info->available[i]);
    }
    puts("}");

    for (int i = 0; i < info->pcount; ++i) {
        printf("{ ");
        for (int j = 0; j < info->rcount; ++j) {
            printf("%d ", info->allocated[i][j]);
        }
        for (int j = 0; j < info->rcount; ++j) {
            printf("%d ", info->max[i][j]);
        }
        puts("}");
    }

    putchar('\n');
}

int readline(FILE *stream, int *values, int max, int terminator) {
    int ch = ' ';
    int count = 0;

    while (!isdigit(ch = fgetc(stream))) {
        if (feof(stream)) {
            return 0;
        }
    }

    for (int i = 0; !feof(stream) && count < max && ch != terminator; ++i) {
        if (isdigit(ch)) {
            ungetc(ch, stream);
            fscanf(stream, "%d", &values[count++]);
        }
        ch = fgetc(stream);
    }

    return count;
}

void readFile(BankersInfo *bankers, const char *filepath) {
    FILE *fp = fopen(filepath, "r");
    bankers->pcount = 0;

    if (fp) {
        bankers->rcount = readline(fp, bankers->available, RESOURCES_MAX, ']');

        for (int i = 0; !feof(fp) && i < PROCESSES_MAX; ++i) {
            int count = readline(fp, bankers->allocated[i], bankers->rcount, ']') +
                        readline(fp, bankers->max[i], bankers->rcount, ']');

            if (count == bankers->rcount * 2) {
                ++bankers->pcount;
            } else {
                break;
            }
        }
    }

    fclose(fp);
}

void bankersAlgorithm(BankersInfo *bankers, int *available, int *finished,
                       int finished_count, int *order) {
    if (finished_count == bankers->pcount) {
        for (int i = 0; i < bankers->pcount; ++i) {
            printf("P%d  ", order[i]);
        }
        putchar('\n');
        return;
    }

    for (int i = 0; i < bankers->pcount; ++i) {
        bool locked = false;

        if (!finished[i]) {
            for (int j = 0; j < bankers->rcount; ++j) {
                if (bankers->max[i][j] - bankers->allocated[i][j] > available[j]) {
                    locked = true;
                    break;
                }
            }

            if (locked) {
                continue;
            }

            int new_available[bankers->rcount];

            for (int j = 0; j < bankers->rcount; ++j) {
                new_available[j] = available[j] + bankers->allocated[i][j];
            }

            finished[i] = true;
            order[finished_count] = i;
            bankersAlgorithm(bankers, new_available, finished, finished_count + 1, order);
            finished[i] = false;
        }
    }
}

int main(void) {
    int finished[PROCESSES_MAX] = {false};
    int order[PROCESSES_MAX] = {0};

    BankersInfo bankers;
    readFile(&bankers, "file.txt");
    printBankers(&bankers);

    bankersAlgorithm(&bankers, bankers.available, finished, 0, order);
}
