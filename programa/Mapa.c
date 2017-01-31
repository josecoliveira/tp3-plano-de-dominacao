#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Mapa.h"

Mapa* criaMapa(int numLinhas, int numColunas) {
    Mapa* mapa = malloc(sizeof(Mapa));
    mapa->numLinhas = numLinhas;
    mapa->numColunas = numColunas;
    mapa->posicao = malloc(sizeof(int*) * numLinhas);
    mapa->threadAtual = 0;
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
} Parametros;

void* paralelizacaoMelhorSoma(void* parametrosAux) {
    Parametros* parametro = (Parametros*) parametrosAux;
    Mapa* mapa = parametro->mapa;
    int *melhoresSomas = parametro->melhoresSomas;
    int linha;
    while ((linha = mapa->threadAtual++) < mapa->numLinhas){
    	melhoresSomas[linha] = melhorSomaLinha(mapa->posicao[linha], mapa->numColunas);
    }
    pthread_exit(NULL);
}

int melhorSomaTotal(Mapa* mapa, int numCores) {

    pthread_t* thread = malloc(sizeof(pthread_t) * numCores);
    int* melhoresSomas = malloc(sizeof(int) * mapa->numLinhas);
    Parametros parametro = {mapa, melhoresSomas};
    for (int i = 0; i < numCores; i++) {
        pthread_create(&thread[i], NULL, paralelizacaoMelhorSoma, &parametro);
    }
    for (int i = 0; i < numCores; i++) {
        pthread_join(thread[i], NULL);
    }
    int somaTotal = melhorSomaLinha(melhoresSomas, mapa->numLinhas);
    free(melhoresSomas);
    free(thread);
    return somaTotal;
}
