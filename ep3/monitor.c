#include "monitor.h"

Food c;                         /* capacidade do pote    */
Food pot_food;
pthread_mutex_t mutex;
pthread_cond_t potfull;         /* sinaliza se pot_food == C */
pthread_cond_t potempty;        /* sinaliza se pot_food == 0 */

int end;
int repetitions;
/*int aux_r;*/

int* s_potempty;

void monitor_init(Food capacity, int r) {
  int i;
  c = capacity;
  pot_food = c;
  end = 0;
  repetitions = r;
  /*aux_r = r;*/
  s_potempty = (int*) malloc(sizeof(int)*r);
  assert(s_potempty);
  for(i=0;i<r;i++){
    s_potempty[i]=-1;
  }
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&potfull,NULL);
  pthread_cond_init(&potempty,NULL);
}
void monitor_finish() {
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&potfull);
	pthread_cond_destroy(&potempty);
	clean_queue();
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
/*void set_end(int value) {
	pthread_mutex_lock(&mutex);
	if(end == 0){
	  end = value;
	} else {
	  send = value;
	  signal_all(&potfull);
	}
      	printf("fim!\n");
	pthread_mutex_unlock(&mutex);
}*/
int get_repetitions() {
  return repetitions;
}
void get_food_from_pot(Food *f, int tid, int weight, int* e) {
  pthread_mutex_lock(&mutex);
  if (end && pot_food == 0){
    pthread_mutex_unlock(&mutex);
    return;
  }
  if (highest_priority_element() > 0){
    insert_with_priority(tid, weight);
    if(highest_priority_element() != tid){
      pthread_mutex_unlock(&mutex);
      return;
    }
    pull_highest_priority_element();
  }
  
  while (pot_food == 0) {
    if(repetitions > 0){
      if(s_potempty[repetitions-1] == -1) s_potempty[repetitions-1]=tid;
    }
    signal_all(&potempty);
    wait(&potfull);
    if (end && pot_food == 0){
      pthread_mutex_unlock(&mutex);
      return;
    }
  }
  *f = 1;
  pot_food -= *f;
  e[0] += 1;
  pthread_mutex_unlock(&mutex);
}
void put_food_in_pot(Food f, int tid,int* c) {
  /*int i;*/
  pthread_mutex_lock(&mutex);
  if (end) {
    pthread_mutex_unlock(&mutex);
    return;
  }
  while (pot_food > 0) {
    wait(&potempty);
     if (end) {
      pthread_mutex_unlock(&mutex);
      return;
    }
  }
  repetitions--;
  if(repetitions < 0){
    if(end == 0){
      end = 1;
      signal_all(&potempty);
      signal_all(&potfull);
    }
  } else {
    pot_food = f;
    signal_all(&potfull);
    c[0] += 1;
    /*printf("s_potempty:");
    for(i=0;i<aux_r;i++){
      printf(" %d",s_potempty[i]);
      }
      putchar('\n');*/
    printf("Selvagem que notou que o pote está vazio: %d\n",s_potempty[repetitions]);
    printf("Cozinheiro que encheu o pote: %d\n",tid);
    print();
  }
  pthread_mutex_unlock(&mutex);
}
