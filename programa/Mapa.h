#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

typedef struct {
    int numLinhas;
    int numColunas;
    int **posicao;
} Mapa;

Mapa* criaMapa(int numLinhas, int numColunas);

int* getEnderecoPosicao(Mapa* mapa, int linha, int coluna);

void liberaMapa(Mapa* mapa);

int melhorSomaTotal(Mapa* mapa, int numCores);

#endif // MAPA_H_INCLUDED
