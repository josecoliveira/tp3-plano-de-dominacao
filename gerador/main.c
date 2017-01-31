#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[]) {
  int linhas, colunas;

  srand(time(NULL));
  int r;

  scanf("%d %d", &linhas, &colunas);
  printf("%d %d\n", linhas, colunas);

  for (int i = 0; i < linhas; i++) {
    for (int j = 0; j < colunas; j++) {
      r = rand();
      printf("%d ", r % 101);
    }
    printf("\n");
  }


  return 0;
}
