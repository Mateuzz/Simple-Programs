#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

#define ADD_WRAP(number, value, limit) (number = (number + value) % limit)

typedef struct {
    int weight;
    int parentNode;
} Path;

Path *findShortestPath(int start, int graph[][7])
{
    Path *path = malloc(sizeof(Path) * 7);
    int nodesToCheck[7], begin = 0, end = 1;
    int cur = start;

    nodesToCheck[0] = start;

    for (int i = 0; i < 7; ++i) {
        path[i].weight = INT_MAX;
        path[i].parentNode = -1;
    }

    path[start].weight = 0;

    while (end != begin) {
        for (int nodeTo = 0; nodeTo < 7; ++nodeTo) {
            if (graph[cur][nodeTo] == 0) {
                continue;
            }

            int weightSum = path[cur].weight + graph[cur][nodeTo];

            if (weightSum < path[nodeTo].weight) {
                path[nodeTo].weight = weightSum;
                path[nodeTo].parentNode = cur;

                int i;
                for (i = begin + 1; i < end; ADD_WRAP(i, 1, 7)) {
                    if (nodeTo == nodesToCheck[i]) {
                        break;
                    }
                }
                if (i >= end) {
                    nodesToCheck[end] = nodeTo;
                    ADD_WRAP(end, 1, 7);
                }
            }
        }

        cur = nodesToCheck[ADD_WRAP(begin, 1, 7)];
    }

    return path;
}

void printGraph(int graph[][7]) {
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (graph[i][j]) {
                printf("%d => %d -- weight = %d\n", i, j, graph[i][j]);
            }
        }
    }
}

int main() {
    int graph[7][7] = {
        {0, 4, 5, 0, 0, 0, 0},
        {4, 0, 2, 3, 0, 0, 0},
        {5, 2, 0, 7, 1, 0, 0},
        {0, 3, 7, 0, 2, 2, 0},
        {0, 0, 1, 2, 0, 10, 9},
        {0, 0, 0, 2, 10, 0, 5},
        {0, 0, 0, 0, 9, 5, 0},
    };

    printGraph(graph);

    Path *path = findShortestPath(6, graph);

    int curNumber = 0;
    Path cur;

    while (curNumber != -1) {
        cur = path[curNumber];
        printf("Current = %d -- total weight = %d\n", curNumber, cur.weight);
        curNumber = cur.parentNode;
    }

    free(path);
}
