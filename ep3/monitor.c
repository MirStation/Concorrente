#include "monitor.h"

Food c;                         /* capacidade do pote    */
Food pot_food;
pthread_mutex_t mutex;
pthread_cond_t potfull;         /* sinaliza se pot_food == C */
pthread_cond_t potempty;        /* sinaliza se pot_food == 0 */

int end;
char weight_option;
int repetitions;

void monitor_init(Food capacity, char w_option, int r) {
	c = capacity;
	pot_food = 0;
	end = 0;
	repetitions = r;
	weight_option = w_option;
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
void get_food_from_pot(Food *f, int tid, int weight) {
  pthread_mutex_lock(&mutex);
  if (end && pot_food == 0){
    pthread_mutex_unlock(&mutex);
    return;
  }
  while (pot_food == 0) {
    signal_all(&potempty);
    wait(&potfull);
    if (end && pot_food == 0){
      pthread_mutex_unlock(&mutex);
      return;
    }
  }
  printf("Selvagem %d servido...\n",tid);
  *f = 1;
  pot_food -= *f;
  printf("Selvagem %d comendo ...\n",tid);
  pthread_mutex_unlock(&mutex);
}
void put_food_in_pot(Food f, int tid) {
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
    printf("Cozinheiro que encheu o pote: %d\n",tid);
  }
  pthread_mutex_unlock(&mutex);
}
