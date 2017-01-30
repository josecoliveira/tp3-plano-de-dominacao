#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

typedef struct {
    int numLinhas;
    int numColunas;
    int **posicao;
} Mapa;

Mapa* criaMapa(int numLinhas, int numColunas);

void liberaMapa(Mapa* mapa);

int* getEnderecoPosicao(Mapa* mapa, int linha, int coluna);

int getValorPosicao(Mapa* mapa, int linha, int coluna);

void imprimeMapa(Mapa* mapa);

int melhorSomaLinha(int* linha, int tamanho);

int melhorSomaTotal(Mapa* mapa);

#endif // MAPA_H_INCLUDED
