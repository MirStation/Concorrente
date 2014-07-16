#include <stdlib.h>
#include "tipo.h"
#include "global.h"

int *ordena_pilotos_por_pontuacao() {
  int i, k, delta = 0;
  int limit = NCORRIDAS * pontos[0], max = 0, *max_id = NULL;
  max_id = (int *) malloc(sizeof(int)*n_pilotos);
  assert(max_id);
  for (k = 0; k < n_pilotos; ++k, limit = max) {
    for (i = 0, max = 0; i < n_pilotos; ++i) {
      if (limit > pilotos[i].pontuacao && (k == 0 || i != max_id[k-1])) {
	if (max < pilotos[i].pontuacao) {
	  max = pilotos[i].pontuacao;
	  max_id[k] = i;
	  delta = 0;
	} else if (max == pilotos[i].pontuacao) {
	  delta++;
	  max_id[k+delta] = i;
	}
      }
    }	
    k += delta;
    delta = 0;
  }
  return max_id;				
}

int* ordena_equipes_por_pontuacao () {
  int i,j,tmp;
  int *equipes_ids;
  int *equipes_pontuacoes = (int *) calloc(n_pilotos/2,sizeof(int));
  assert(equipes_pontuacoes);
  for(i = 0; i < n_pilotos; i++){
    equipes_pontuacoes[i/2] += pilotos[i].pontuacao;
  }
  equipes_ids = (int *) malloc(sizeof(int)*(n_pilotos/2));
  assert(equipes_ids);
  for(i = 0; i < (n_pilotos/2); i++){
    equipes_ids[i] = i;
  }
  for(i = 1; i < (n_pilotos/2); i++){
    for(j = i; j > 0 && equipes_pontuacoes[j-1]<equipes_pontuacoes[j];j--){
      tmp = equipes_pontuacoes[j];
      equipes_pontuacoes[j]= equipes_pontuacoes[j-1];
      equipes_pontuacoes[j-1]=tmp;
      tmp = equipes_ids[j];
      equipes_ids[j]= equipes_ids[j-1];
      equipes_ids[j-1]=tmp;
    }
  }
  free(equipes_pontuacoes);
  return equipes_ids;
}
