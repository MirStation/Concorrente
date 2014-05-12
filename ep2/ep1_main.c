#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "global.h"
#include "tipo.h"
#include "leitura.h"
#include "init_pilotos.h"
#include "init_pista.h"
#include "sort.h"

void imprime_pilotos(int *max_id);
void imprime_equipes(int *max_id);
void imprime_pista();

void define_new_pos(int x, int y, int *new_x, int *new_y) {
  *new_x = x;
  *new_y = y;
  if(x == NPISTA-1) {
    if(pista[0][0] == LIVRE) {
      *new_x = 0; 
      *new_y = 0;
    }
    else if(pista_dupla[0] && pista[0][1] == LIVRE) {
      *new_x = 0; 
      *new_y = 1;
    }
  }
  else {
    if(pista[x+1][0] == LIVRE) {
      *new_x += 1; 
      *new_y = 0;
    }
    else if(pista_dupla[x+1] && pista[x+1][1] == LIVRE){
      *new_x += 1; 
      *new_y = 1;
    }
  }	
}

void define_new_pos_box(int x, int y, int *new_x, int *new_y) {
  *new_x = x;
  *new_y = y;
  if(y != NO_BOX && boxes[0] == LIVRE) {
    *new_x = 0; 
    *new_y = NO_BOX;
  }
  else if(boxes[x+1] == LIVRE) {
    *new_x += 1; 
  }
}		

void *Piloto(void *argument) {
  bool chegou = FALSE , espera = FALSE, box_wait = FALSE;
  int tid;
  int x, y, new_x, new_y;
  int volta = 0;
  int tempo_abast = 0, tempo_no_box = 0;
  piloto *p;
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  tid = *((int *) argument);
  p = &pilotos[tid];
  x = p->posx;
  y = p->posy;
  while (chegada < n_pilotos) {
    pthread_mutex_lock(&lock);
    if(!p->abastecido && !p_box[tid]) {
      if(x == BOXINI-1 && volta >= voltas/2) {
	if(!p_box[2*(tid/2) + (tid + 1)%2]) 
	  p_box[tid] = TRUE;
      }
      if(!p_box[tid] && volta == (voltas/2)+2) { p->abastecido = TRUE; printf("%d morreu\n",tid); }
    }
    if(p->reduz && volta >= voltas-10 && !espera && !box_wait) {		
      espera = TRUE;
    }
    else if (box_wait) { espera = TRUE; box_wait = FALSE; tempo_no_box++; }
    else if (tempo_abast > 0) tempo_abast--;
	else {
		if(p->reduz) espera = FALSE;
		if(p_box[tid] == FALSE) {
			define_new_pos(x,y,&new_x,&new_y); 
			if(x != new_x) {
				if(new_x == 0){ 
					volta++;
					if (volta > volta_atual && volta <= voltas) {
						volta_atual = volta;
						pi = 0;
						podio[0] = -1;
						podio[1] = -1;
						podio[2] = -1;
					}
					if (pi < 3 && volta == volta_atual && volta <= voltas) {
						podio[pi] = tid;
						pi++;
					}
				}
				pista[x][y]=LIVRE;
				pista[new_x][new_y]=tid;
				x = new_x;
				y = new_y;
			}
		}
		else {
			tempo_no_box++;
			box_wait = TRUE;
			if(y != NO_BOX) {
				define_new_pos_box(x,y,&new_x,&new_y);
				if(y != new_y) {
					pista[x][y] = LIVRE;
					x = new_x;	
					y = new_y;
					boxes[new_x]=tid;
				}
			}
			else if(x < BOXCMPR-1) {
				define_new_pos_box(x,y,&new_x,&new_y);
				if(x != new_x) {
					boxes[x]=LIVRE;
					boxes[new_x]=tid;
					x = new_x;	
				}			
			}
			else {
				define_new_pos(BOXINI+BOXCMPR-1,y,&new_x,&new_y);
				if(y != new_y) {
					boxes[x]=LIVRE;
					pista[new_x][new_y]=tid;
					x = new_x;	
					y = new_y;
					p->abastecido = TRUE;
					p_box[tid] = FALSE;
				 }	
				 printf("[BOXES] %d ficou %2.1f segundos no box.\n",tid,tempo_no_box/2.0); 
			}
			if(y == NO_BOX && x == BOXCMPR/2) {
				tempo_abast = rand()%12 + 8;
				tempo_no_box += tempo_abast;
				/*printf("%d esperou %2.1f segundos.\n",tid,tempo_abast/2.0); */
			}
		}
    }
    if(volta == voltas && !chegou) {
      if(chegada < NPONTO) p->pontuacao += pontos[chegada];
      printf("%3d terminou +%2d\n",tid,(chegada < NPONTO)?pontos[chegada]:0);
      chegada++;
      chegou = TRUE;
    }
    pthread_mutex_unlock(&lock);			
    chegar[tid]=1;
    while(continuar[tid]==0) { 
      tim.tv_nsec = rand()%1000;
      nanosleep(&tim,&tim2);
    }
    continuar[tid]=0;
  }
  p = NULL;
  return NULL;
}

void *Observador() {
  int i;
  struct timespec tim, tim2;
  tim.tv_sec = 0;	
  while(chegada < n_pilotos) {
    for(i=0;i<n_pilotos;i++){
      while(chegar[i]==0 && chegada < n_pilotos){
	tim.tv_nsec = rand()%1000;
	nanosleep(&tim,&tim2);
      }
      chegar[i]=0;
    }
    for(i=0;i<n_pilotos;i++){
      continuar[i]=1;
    }
  }
  for(i=0;i<n_pilotos;i++){
    continuar[i]=1;
  }
  return NULL;
}

void *Impressora() {
  int *max_id;
  int i;
  int volta = 0;
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  while(chegada < n_pilotos) {
    if (pi == 3 && volta_atual > volta && volta_atual <= voltas) {
      printf("Podio da volta %d:\n",volta_atual);
      for(i=0;i<3;i++){
	printf("%3d terminou em %3dº lugar\n",podio[i],i+1);
      }
      volta = volta_atual;
    } else {
      tim.tv_nsec = rand()%1000;
      nanosleep(&tim,&tim2);
    }
  }
  max_id = ordena_pilotos_por_pontuacao();
  imprime_pilotos(max_id);
  free(max_id);
  max_id = ordena_equipes_por_pontuacao();
  imprime_equipes(max_id);
  free(max_id);
  return NULL;
}

void imprime_pilotos(int *max_id) {
  int i;
  puts("Classificacao geral por pilotos:");
  for (i = 0; i < n_pilotos; ++i) {
    printf("%3dº lugar com %3d: %3d [%3d]\n",i+1,pilotos[max_id[i]].pontuacao,max_id[i],max_id[i]/2);
  }
}

void imprime_equipes (int *max_id) {
  int i;
  int *equipes_pontuacoes = (int *) calloc(n_pilotos/2,sizeof(int));
  assert(equipes_pontuacoes);
  for(i = 0; i < n_pilotos; i++){
    equipes_pontuacoes[i/2] += pilotos[i].pontuacao;
  }
  puts("Classificacao geral por equipes:");
  for (i = 0; i < (n_pilotos/2); i++) {
    printf("%3dº lugar com %3d: %3d\n",i+1,equipes_pontuacoes[max_id[i]],max_id[i]);
  }
  free(equipes_pontuacoes);
}

void imprime_pista() {
  int i, j;
  for (i=0; i<NPISTA; i+=80) {
    for(j=0; j<80; j++)
      printf("%c",(pista_dupla[i+j])?((pista[i+j][1] == -1)?'-':'p'):' ');
    printf("\n");
    for(j=0; j<80; j++)
      printf("%c",(pista[i+j][0] == -1)?'-':'p');
    printf("\n\n");
  }
}

int main(int argc, char **argv) {
  pthread_t *threads;
  int *thread_args;
  int rc, i, *max_id;
  init_global_variables();
  assert(argc >= 2 && argc <= 3);
  ler_entrada(argc,argv);
  threads = malloc(sizeof(*threads)*(n_pilotos+2)); 
  thread_args = malloc(sizeof(*thread_args)*n_pilotos); 
  pilotos = malloc(sizeof(piloto)*n_pilotos);
  p_box = malloc(sizeof(*p_box)*n_pilotos);
  for (i = 0; i < n_pilotos; ++i) { p_box[i] = FALSE; pilotos[i].abastecido = FALSE; }
  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init failed\n");
    return 1;
  }
  srand(time(NULL));
  define_pontuacao_inicial();
  define_pilotos_k();
  max_id = ordena_pilotos_por_pontuacao();
  imprime_pilotos(max_id);
  init_pista(max_id);
  for (i=0; i<n_pilotos; ++i) {
    thread_args[i] = i;
    rc = pthread_create(&threads[i], NULL, Piloto, (void *) &thread_args[i]);
    assert(0 == rc);
  }
  rc = pthread_create(&threads[i++], NULL, Observador, NULL);
  assert(0 == rc);
  rc = pthread_create(&threads[i], NULL, Impressora, NULL);
  assert(0 == rc);
  for (i=0; i<(n_pilotos+2); ++i) {
    rc = pthread_join(threads[i], NULL);
    assert(0 == rc);      
  }
  free(max_id);
  free(threads);
  free(thread_args);
  free(pilotos);
  free(chegar);
  free(continuar);
  free(podio);
  free(p_box);
  return 0;
}
