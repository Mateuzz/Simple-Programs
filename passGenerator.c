#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long seedVal = 1;

static void SetSeed(unsigned long seed) {
    seedVal = seed;
}

typedef unsigned long RandomFunction(void);
typedef RandomFunction *RandomFunctionPtr;

static unsigned long GetRandom(void) {
    seedVal = seedVal * 1103515245 + 12345;
    return seedVal;
}

static unsigned long Hash(const char *word) {
    unsigned long hashval = 0;
    
    for (int i = 0; word[i]; ++i) {
        hashval += word[i] * (i + 1) + 1234567;
    }
    
    return hashval;
}

#define SENHA_MAX 30

static char *GerarSenha(RandomFunctionPtr randomGenerator) {
    static const char *simbolos = "12345678cbvMZ90FGqwerRtyuioplkjhgfdsazmxn"
                                  "NXBCVLAKSJDHPQOWIEUYT!@#$%%&*()_-=+<>][{}";
    static size_t max = 0;
    if (max == 0) {
        max = strlen(simbolos);
    }
    
    char *senha = malloc(SENHA_MAX + 1);
    
    for (int i = 0; i < SENHA_MAX; ++i) {
        senha[i] = simbolos[randomGenerator() % max];
    }
    
    return senha;
}

static void GerarSeedPalavra(const char *word) {
    SetSeed(Hash(word));
}

int main(int argc, char **argv) {
    if (argc < 2) {
       printf("Uso: programa <palavras>\n");
       return 1;
    }
    
    char outputLabel[100];
    
    for (int i = 1; i< argc; ++i) {
        GerarSeedPalavra(argv[i]);
        
        char *senha = GerarSenha(&GetRandom);

        snprintf(outputLabel, 99, "%s:", argv[i]);
        printf("%-30s %s\n", outputLabel, senha);
        
        free(senha);
    }

#ifdef WIN32
    system("pause");
#endif
}
