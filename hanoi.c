#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef enum {
    NONE = 0,
    BLUE = 1,
    GREEN = 2,
    YELLOW = 3,
    ORANGE = 4,
    RED = 5
} HanoiDisk;

typedef struct {
    int pillar;
    int height;
} DiskPlace;

typedef struct {
    HanoiDisk pillars[3][5];
} HanoiTower;

const char *pillar_name(HanoiDisk color) {
    static const char *colors[] = {
        "none", "red 5", "orange 4", "yellow 3", "green 2", "blue 1"
    };
    return colors[color];
}

void print_tower(const HanoiTower *tower) {
    for (int i = 0; i < 3; ++i) {
        printf("Pillar %d\n", i);
        for (int j = 0; j < 5; ++j) {
            printf("Color: %s\n", pillar_name(tower->pillars[i][j]));
        }
        putchar('\n');
    }
    puts("=======================");
} 

int move_to(HanoiTower *tower, DiskPlace position, int fate) {
    HanoiDisk *place = &tower->pillars[position.pillar][position.height];

    for (int i = 0; i < 5; ++i) {
        if (tower->pillars[fate][i] == NONE &&
                (i == 0 || tower->pillars[fate][i - 1] > *place)) {
            tower->pillars[fate][i] = *place;
            *place = NONE;
            print_tower(tower);
            return i;
        }
    }

    assert(false);
    return -1;
}

bool can_move(HanoiTower *tower, DiskPlace position) {
    return position.height == 4 || tower->pillars[position.pillar][position.height + 1] == NONE;
}

int get_above_fate(int current, int fate) {
    for (int i = 0; i < 3; ++i) {
        if (i != current && i != fate) {
            return i;
        }
    }
}

DiskPlace resolve(HanoiTower *tower, DiskPlace from, int to) {
    if (can_move(tower, from)) {
        int height_to = move_to(tower, from, to);
        return (DiskPlace) { to, height_to };
    }

    DiskPlace above = {
        from.pillar,
        from.height + 1
    };

    DiskPlace above_to = resolve(tower, above, get_above_fate(from.pillar, to));
    int height_to = move_to(tower, from, to);
    resolve(tower, above_to, to);
    return (DiskPlace) { to, height_to };
}


int main(void) {
    HanoiTower tower = { {{RED, ORANGE, YELLOW, GREEN, BLUE}, {}, {}} };
    print_tower(&tower);
    resolve(&tower, (DiskPlace) {0, 0}, 2);
}
