#include <stdio.h>
#include <stdlib.h>
#include "tipo.h"
#include "global.h"

int rand_piloto() {
  return rand()%n_pilotos;
}

void define_pontuacao_inicial() {
  int i, j, k;
  int *corrida;
  corrida = malloc(sizeof(* corrida)*n_pilotos);
  for (i=0; i<n_pilotos; ++i) {
    corrida[i] = 0;
  }
  srand(time(NULL));
  for (i=0; i<n_pilotos; ++i) {
    pilotos[i].pontuacao = 0;
  }
  for (j=0; j<19 ; ++j) {
    for (i=0; i<n_pilotos && i<NPONTO; ++i) {
      do { k = rand_piloto();} while(corrida[k]);
      corrida[k] = TRUE;
      pilotos[k].pontuacao += pontos[i];
    }
    for (i=0; i<n_pilotos; ++i) {
      corrida[i] = FALSE;
    }
  }
  free(corrida);
}

void define_pilotos_k() {
  int n = (int) ((1.0 * n_pilotos * k)/100);
  int i, j;
  bool *escolhidos;
  escolhidos = malloc(sizeof(* escolhidos)*n_pilotos);
  for (i = 0; i < n_pilotos; ++i) {
    pilotos[i].reduz = FALSE;
  }
  for (i = 0; i < n; ++i) {
    do { j = rand_piloto(); } while(escolhidos[j]); 
    escolhidos[j] = TRUE;
    pilotos[j].reduz = TRUE;
    printf("%3d escolhido\n", j);
  }
  free(escolhidos);
}
