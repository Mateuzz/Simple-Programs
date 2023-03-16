#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned long long u64;

u64 decimal_para_binario(u64 n) {
    return (n / 2) ? decimal_para_binario(n / 2) * 10 + n % 2 : n % 2;
}

u64 binario_para_decimal(u64 n, u64 e) {
    return (n) ? binario_para_decimal(n / 10, e + 1) + ((n % 10) * pow(2, e)) : 0;
}

#define binario_para_decimal(n) binario_para_decimal(n, 0)

int main(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        printf("%s = %llu\n", argv[i], binario_para_decimal(strtoll(argv[i], NULL, 10)));
    }
}
