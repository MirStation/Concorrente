#include "global.h"

void init_pista(int *ordem_pilotos) {
  int i, j;
  for (i=0; i<NPISTA; ++i) {
    pista[i][0] = LIVRE;
    pista[i][1] = LIVRE;
  }
  for (i=0; i<NBOX; ++i) {
    boxes[i] = LIVRE;
  }
  for (i=0, j=NPISTA-1; j>=0 && i<n_pilotos; --j, ++i) {
    pista[j][0] = ordem_pilotos[i];
    pilotos[ordem_pilotos[i]].posx = j;
    pilotos[ordem_pilotos[i]].posy = 0;
    if(pista_dupla[j] && i+1<n_pilotos) {
      i++;
      pista[j][1] = ordem_pilotos[i];	
      pilotos[ordem_pilotos[i]].posx = j;
      pilotos[ordem_pilotos[i]].posy = 1;
    }
  }
}
