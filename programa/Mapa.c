#include <stdio.h>
#include <stdlib.h>
#include "Mapa.h"

Mapa* criaMapa(int numLinhas, int numColunas) {
    Mapa* mapa = malloc(sizeof(Mapa));
    mapa->numLinhas = numLinhas;
    mapa->numColunas = numColunas;
    mapa->posicao = malloc(sizeof(int*) * numLinhas);
    for (int i = 0; i < numLinhas; i++) {
        mapa->posicao[i] = malloc(sizeof(int *) * numColunas);
    }
    return mapa;
}

void liberaMapa(Mapa* mapa) {
    for (int i = 0; i < mapa->numLinhas; i++) {
        free(mapa->posicao[i]);
    }
    free(mapa->posicao);
    free(mapa);
}

int* getEnderecoPosicao(Mapa* mapa, int linha, int coluna) {
    int* endereco = &mapa->posicao[linha][coluna];
    return endereco;
}

int getValorPosicao(Mapa* mapa, int linha, int coluna) {
    int valor = mapa->posicao[linha][coluna];
    return valor;
}

void imprimeMapa(Mapa* mapa) {
    for (int i = 0; i < mapa->numLinhas; i++) {
        for (int j = 0; j < mapa->numColunas; j++) {
            printf("%d ", mapa->posicao[i][j]);
        }
        printf("\n");
    }
}

int melhorSomaLinha(int* linha, int tamanho) {
    int a = 0, b = 0, posicaoAtual = tamanho - 1;
    int auxB;
    while (posicaoAtual >= 0) {
        if (a >= b + linha[posicaoAtual]) {
            b = a;
        } else {
            auxB = b;
            b = a;
            a = auxB + linha[posicaoAtual];
        }
        posicaoAtual--;
    }
    return a;
}

int melhorSomaTotal(Mapa* mapa) {
    int* melhoresSomas = malloc(sizeof(int) * mapa->numLinhas);
    for (int i = 0; i < mapa->numLinhas; i++) {
        melhoresSomas[i] = melhorSomaLinha(mapa->posicao[i], mapa->numColunas);
    }
    int somaTotal = melhorSomaLinha(melhoresSomas, mapa->numLinhas);
    free(melhoresSomas);
    return somaTotal;
}
