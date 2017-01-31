#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Mapa.h"

int main(int argc, char const *argv[]) {
    ///Entrada
    int numLinhas, numColunas;
    int valid = 0;
    scanf("%d", &numLinhas);
    scanf("%d", &numColunas);

    Mapa* mapa = criaMapa(numLinhas, numColunas);
    printf("Criou mapa\n");
    int i, j;
    for (i = 0; i < numLinhas; i++) {
        for (j = 0; j < numColunas; j++) {
            scanf("%d", getEnderecoPosicao(mapa, i, j));
        }
    }

    int melhorSoma;
    int numCores = atoi(argv[1]);

    melhorSoma = melhorSomaTotal(mapa, numCores);

    printf("%d\n", melhorSoma);

    ///Liberação de memória
    liberaMapa(mapa);
    return 0;
}
