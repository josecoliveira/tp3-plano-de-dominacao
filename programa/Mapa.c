#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

typedef struct {
    Mapa* mapa;
    int* melhoresSomas;
    int limiteInferior;
    int limiteSuperior;
} Parametros;

void* paralelizacaoMelhorSoma(void* parametrosAux) {
    Parametros* parametros = (Parametros*) parametrosAux;
    for (int i = parametros->limiteInferior; i <= parametros->limiteSuperior; i++) {
        parametros->melhoresSomas[i] = melhorSomaLinha(parametros->mapa->posicao[i], parametros->mapa->numColunas);
    }
    pthread_exit(NULL);
}

int melhorSomaTotal(Mapa* mapa, int numCores) {
    int i;

    pthread_t thread[numCores];
    int* melhoresSomas = malloc(sizeof(int) * mapa->numLinhas);
    Parametros* parametros = malloc(sizeof(Parametros) * numCores);

    if (numCores >= mapa->numLinhas) {
        for (i = 0; i < mapa->numLinhas; i++) {
            parametros[i].mapa = mapa;
            parametros[i].melhoresSomas = melhoresSomas;
            parametros[i].limiteInferior = i;
            parametros[i].limiteSuperior = i;
            pthread_create(&thread[i], NULL, paralelizacaoMelhorSoma, &parametros[i]);
        }
        for (i = 0; i < mapa->numLinhas; i++) {
            pthread_join(thread[i], NULL);
        }
    } else if (numCores < mapa->numLinhas) {
        int slice = mapa->numLinhas / numCores;
        for (i = 0; i < numCores - 1; i++) {
            parametros[i].mapa = mapa;
            parametros[i].melhoresSomas = melhoresSomas;
            parametros[i].limiteInferior = i * slice;
            parametros[i].limiteSuperior = (i + 1) * slice;
            pthread_create(&thread[i], NULL, paralelizacaoMelhorSoma, &parametros[i]);
        }
        parametros[i].mapa = mapa;
        parametros[i].melhoresSomas = melhoresSomas;
        parametros[i].limiteInferior = i * slice;
        parametros[i].limiteSuperior = mapa->numLinhas - 1;
        pthread_create(&thread[i], NULL, paralelizacaoMelhorSoma, &parametros[i]);
        for (i = 0; i < numCores; i++) {
            pthread_join(thread[i], NULL);
        }
    }

    int somaTotal = melhorSomaLinha(melhoresSomas, mapa->numLinhas);
    free(melhoresSomas);
    return somaTotal;
    return 1;
}
