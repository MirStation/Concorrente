#include "global.h"

void init_global_variables() {
  int pts[] = {25,18,15,12,10,8,6,4,2,1};
  int i;
  for(i = 0; i < NPONTO; ++i) {
    pontos[i] = pts[i];
  }
  voltas = 0;
  equipes = 0;
  n_pilotos = 0;
  chegada = 0;
  opcao = 0;
  k = 0;
  debug = FALSE;
  chegar = NULL;
  continuar = NULL;
  pi = 0;
  podio = (int*) malloc(3*sizeof(int));
  assert(podio);
  volta_atual = 0;
}
