#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#include "tipo.h"

#ifndef GLOBAL
#define GLOBAL

#define NCORRIDAS 20

piloto *pilotos;
bool *p_box;

#define NPISTA 160
#define LIVRE -1

int pista[NPISTA][2];
bool pista_dupla[NPISTA];

#define BOXINI 50
#define BOXCMPR 10
#define NBOX 10
#define NO_BOX -1
int boxes[NBOX];

#define NPONTO 10
int pontos[10];

int voltas;
int equipes;
int n_pilotos;
short int chegada;
char opcao;
int k;

bool debug;
pthread_mutex_t lock;

int *chegar; 
int *continuar;

int pi;
int volta_atual;
int *podio;

void init_global_variables();

#endif
