#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "global.h"
#include "tipo.h"

void ler_trechos(FILE *file) {
  char line[64];
  int begin, end;
  int i;  
  while (fgets(line, 64, file) != NULL) {
    sscanf (line, "%d", &begin);
    if (fgets(line, 64, file) != NULL) {
      sscanf (line, "%d", &end);
    } else {
      printf("\nformato errado no arquivo de entrada.\n\n");
      exit(0);
    }	
    for(i = begin; i <= end; i++){
      pista_dupla[i] = TRUE;
    }
  }
}

void ler_entrada(int argc, char **argv) {
  FILE *file;
  file = fopen(argv[1],"r");
  if(file == NULL) { 
    printf("\n[ERRO] arquivo inexistente\n\n"); 
    exit(0);
  }
  fscanf(file,"%d\n",&voltas);
  fscanf(file,"%d\n",&equipes);
  n_pilotos = 2*equipes;
  chegar = (int *) calloc(n_pilotos,sizeof(int));
  assert(chegar);
  continuar = (int *) calloc(n_pilotos,sizeof(int));
  assert(continuar);
  fscanf(file,"%c\n",&opcao);
  if (opcao == 'A') {
    fscanf(file,"%d\n",&k);
  }
  ler_trechos(file); 
  if(argc == 3) {
    if(strcmp(argv[2],"-d") == 0){
      debug = TRUE;
    } else {
      printf("\nOpção inválida.\n\n");	
      exit(0);
    }
  }
  fclose(file);
}
