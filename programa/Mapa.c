#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Mapa.h"

/* Função: Construtor para uma estrutura Mapa.
 * Entrada: Número de linhas e colunas no mapa.
 * Saída: Ponteiro para o Mapa.
 */
Mapa* criaMapa(int numLinhas, int numColunas) {
    Mapa* mapa = malloc(sizeof(Mapa));
    mapa->numLinhas = numLinhas;
    mapa->numColunas = numColunas;
    mapa->posicao = malloc(sizeof(int*) * numLinhas);
    mapa->linhaAtual = 0;
    for (int i = 0; i < numLinhas; i++) {
        mapa->posicao[i] = malloc(sizeof(int *) * numColunas);
    }
    return mapa;
}

/* Função: Libera memória alocada por um Mapa.
 * Entrada: Ponteiro para Mapa.
 * Saída: Nenhuma.
 */
void liberaMapa(Mapa* mapa) {
    for (int i = 0; i < mapa->numLinhas; i++) {
        free(mapa->posicao[i]);
    }
    free(mapa->posicao);
    free(mapa);
}

/* Função: Ler endereço da memória de uma posição específica no Mapa.
 * Entrada: Ponteiro para mapa, linha e coluna da posição.
 * Saída: Endereço na memória da posição.
 */
int* getEnderecoPosicao(Mapa* mapa, int linha, int coluna) {
    int* endereco = &mapa->posicao[linha][coluna];
    return endereco;
}

/* Função: Encontrar a melhor soma em um vetor.
 * Entrada: Ponteiro para o vetor e o tamanho do vetor.
 * Saída: Melhor soma.
 */
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

/* Função: Distribuir as linhas do Mapa para os núcleos de processamento.
 * Entrada: Estrutura com o ponteiro para Mapa e o vetor de melhores somas.
 * Saída: Nenhuma.
 */
void* paralelizacaoMelhorSoma(void* parametrosAux) {
    Parametros* parametro = (Parametros*) parametrosAux;
    Mapa* mapa = parametro->mapa;
    int *melhoresSomas = parametro->melhoresSomas;
    int linha;
    while ((linha = mapa->linhaAtual++) < mapa->numLinhas){
    	melhoresSomas[linha] = melhorSomaLinha(mapa->posicao[linha], mapa->numColunas);
    }
    pthread_exit(NULL);
}

/* Função: Encontrar a melhor soma no Mapa.
 * Entrada: Ponteiro para o mapa e o número de núcleos de processamento.
 * Saída: Soma total das posições escolhidas.
 */
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
