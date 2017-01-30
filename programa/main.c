#include <stdio.h>
#include <stdlib.h>
#include "Mapa.h"

int main(int argc, char const *argv[]) {
    ///Entrada
    int numLinhas, numColunas;
    scanf("%d", &numLinhas);
    scanf("%d", &numColunas);
    Mapa* mapa = criaMapa(numLinhas, numColunas);
    for (int i = 0; i < numLinhas; i++) {
        for (int j = 0; j < numColunas; j++) {
            scanf("%d", getEnderecoPosicao(mapa, i, j));
        }
    }

    int melhorSoma = melhorSomaTotal(mapa);

    printf("%d\n", melhorSoma);

    ///Liberação de memória
    liberaMapa(mapa);
    return 0;
}
