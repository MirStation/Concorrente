#include "monitor.h"

Food c;                         /* capacidade do pote    */
Food pot_food;
pthread_mutex_t mutex;
pthread_cond_t potfull;         /* sinaliza se pot_food == C */
pthread_cond_t *potempty_cook;  /* sinaliza se pot_food == 0 */

int end;
int repetitions;
int aux_r;
int n_cooks;
int first;
int* s_potempty;

void monitor_init(Food capacity, int r, int n) {
  int i;
  srand(time(NULL));
  c = capacity;
  pot_food = c;
  end = 0;
  first = 0;
  repetitions = r;
  aux_r = r;
  n_cooks = n;
  s_potempty = (int*) malloc(sizeof(int)*r);
  assert(s_potempty);
  for(i=0;i<r;i++){
    s_potempty[i]=-1;
  }
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&potfull,NULL);
  potempty_cook = malloc(sizeof(*potempty_cook)*n_cooks);
  assert(potempty_cook);
  for(i=0;i<n_cooks;i++)
	pthread_cond_init(&potempty_cook[i],NULL);
}
void monitor_finish() {
	int i;
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&potfull);
	for(i=0;i<n_cooks;i++)
		pthread_cond_destroy(&potempty_cook[i]);
	clean_queue();
	free(s_potempty);
	free(potempty_cook);
}
void wait(pthread_cond_t *cond) {
	pthread_cond_wait(cond,&mutex);
}
void signal(pthread_cond_t *cond) {
	pthread_cond_signal(cond);
}
void signal_all(pthread_cond_t *cond) {
	pthread_cond_broadcast(cond);
}

int get_repetitions() {
  return repetitions;
}
void get_food_from_pot(Food *f, int tid, int weight, int* e) {
  int pt;
  pthread_mutex_lock(&mutex);
  /*puts("entrando pot");*/
  if (end && pot_food == 0){
    pthread_mutex_unlock(&mutex);
    return;
  }
  pt = tid;
  while (pot_food == 0 || pt != tid) {
	if (first == 0) {
		first = 1;
		if(repetitions > 0){
		  if(s_potempty[repetitions-1] == -1) {s_potempty[repetitions-1]=tid;}
		}
		signal(&potempty_cook[uniform_distribution(0,n_cooks-1)]);
	}
	/*puts("IBefore");
	  print_pqueue();*/
	insert_with_priority(tid, weight);
	/*puts("IAfter");
      print_pqueue();*/
    wait(&potfull);
    if (end && pot_food == 0){
      pthread_mutex_unlock(&mutex);
      return;
    }
    /*printf("hp:%d\n",highest_priority_element());*/
      pt = highest_priority_element();
    if(pt == tid){
      /*puts("PBefore");
	print_pqueue();*/
      pull_highest_priority_element();
      /*puts("PAfter");
	print_pqueue();*/
    }
    /*printf("e:%d r:%d pf:%d tid:%d pt:%d\n",end, repetitions,pot_food,tid,pt);*/
  }
  *f = 1;
  pot_food -= *f;
  e[0] += 1;
  /*puts("saindo pot");*/
  if(end==1 && repetitions<0){
    signal_all(&potfull);
  }
  pthread_mutex_unlock(&mutex);
}
void put_food_in_pot(Food f, int tid, int* c) {
  int i;
  pthread_mutex_lock(&mutex);
  /*puts("entando food");*/
  if (end) {
    pthread_mutex_unlock(&mutex);
    return;
  }
  while(first == 0) {
    wait(&potempty_cook[tid]);
     if (end) {
      pthread_mutex_unlock(&mutex);
      return;
    }
  }
  repetitions--;
  if(repetitions < 0){
    if(end == 0){
      end = 1;
      for(i=0;i<n_cooks;i++)
		signal_all(&potempty_cook[i]);
      signal_all(&potfull);
    }
  } else {
    pot_food = f;
    first = 0;
    signal_all(&potfull);
    c[0] += 1;
    /*
    printf("s_potempty:");
    
    for(i=0;i<aux_r;i++){
      printf(" %d",s_potempty[i]);
      }

    putchar('\n');
    printf("rep: %d\n",repetitions);
    if(s_potempty[repetitions]==-1) {puts("erro no indice");exit(1);}
    */
    printf("Selvagem que notou que o pote est� vazio: %d\n",s_potempty[repetitions]);
    printf("Cozinheiro que encheu o pote: %d\n",tid);
    print();
  }
  /*puts("saindo food");*/
  pthread_mutex_unlock(&mutex);
}
